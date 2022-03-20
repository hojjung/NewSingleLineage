#include "MyMovement.h"
#include "NavigationSystem.h"
#include "NavLinkCustomComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "Navigation/NavLinkProxy.h"

UMyMovement::UMyMovement(const FObjectInitializer& obj)
{
	MaxSpeed = 1200.f;
	Acceleration = 4000.f;
	Deceleration = 8000.f;
	bUseAccelerationForPaths = true;
	bUseFixedBrakingDistanceForPaths = true;
	TurningBoost = 8.0f;
	bPositionCorrected = false;

	NavAgentProps.AgentHeight = 88;
	NavAgentProps.AgentRadius = 34;

	ResetMoveState();
}

void UMyMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	UMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	ApplyControlInputToVelocity(DeltaTime);

	LimitWorldBounds();

	bPositionCorrected = false;

	FVector Delta = (Velocity * DeltaTime) + m_ImpactVector;

	if (!Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		const FQuat Rotation = UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);

			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}

		if (!bPositionCorrected)
		{
			const FVector NewLocation = UpdatedComponent->GetComponentLocation();
			Velocity = ((NewLocation - OldLocation) / DeltaTime);
		}
	}

	m_ImpactVector = FVector::ZeroVector;
	
	UpdateComponentVelocity();

	TickRotate(DeltaTime);
}

void UMyMovement::SetImpact(FVector v)
{
	m_ImpactVector = v;
}

void UMyMovement::SetActive(bool new_active, bool reset)
{
	Super::SetActive(new_active, reset);

	if(!new_active)
	{
		m_ImpactVector = FVector::ZeroVector;
	}
}

const INavigationDataInterface* UMyMovement::GetNavData() const
{
	const UWorld* World = GetWorld();

	if (World == nullptr || World->GetNavigationSystem() == nullptr)
	{
		return nullptr;
	}

	const INavigationDataInterface* NavData = FNavigationSystem::GetNavDataForActor(*GetPawnOwner());

	return NavData;
}

void UMyMovement::TickRotate(float deltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	FRotator DesiredRotation = CurrentRotation;

	DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation);
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
	DesiredRotation.Roll = 0.f;

	if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, 1e-3f))
	{
		DesiredRotation.Yaw = UKismetMathLibrary::RInterpTo(CurrentRotation, DesiredRotation, deltaTime, 8).Yaw;

		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, false);
	}
}

FRotator UMyMovement::ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const
{
	if (Velocity.IsNearlyZero(0.01f))
		//회전각이 0이여서 // 몬스터의 경우 추적 대상이 존재한다면 추적대상을 바라봐야함,이함수랑 별개로 만들어야할듯? ㄴㄴ 그냥 움직일때는 고개돌리는게 맞을듯
	{
		return CurrentRotation;
	}

	return Velocity.GetSafeNormal().Rotation();
}
