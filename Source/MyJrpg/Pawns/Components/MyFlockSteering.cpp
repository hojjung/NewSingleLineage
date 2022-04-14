#include "MyFlockSteering.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MonsterPawn.h"


void UMyFlockSteering::BeginPlay()
{
	Super::BeginPlay();
	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);

	m_AryIgnoreActors.Reset();
	m_AryIgnoreActors.Add(GetOwner());
}

bool UMyFlockSteering::GetNeighborPawns(TArray<AActor*>& aryOut)
{
	if(!UMyLib::SphereOverlapActors(GetOwner(),GetOwner()->GetActorRotation(),GetOwner()->GetActorLocation(),500,
		m_AryTargetingObjectType,AMonsterPawn::StaticClass(),m_AryIgnoreActors,aryOut))
	{
		return false;
	}
	return true;
}

FVector UMyFlockSteering::GetFlockDir()
{
	FVector TargetLoc = GetOwner()->GetActorLocation();

	TArray<AActor*> Mobs;

	GetNeighborPawns(Mobs);
	
	FVector Sum = FVector(0);

	int Count = 0;

	for (AActor* OtherActor : Mobs)
	{
		AMonsterPawn* Other = Cast<AMonsterPawn>(OtherActor);
		
		if(!Other->IsAlive() || Other == GetOwner())
		{
			continue;
		}
		FVector OtherLoc = Other->GetActorLocation();
		
		float Dist = FVector::DistSquared2D(TargetLoc, OtherLoc);

		if ((Dist > 0) && (Dist < 90000))
		{
			FVector Diff = TargetLoc - OtherLoc;
			
			Diff = Diff.GetSafeNormal2D();

			Sum += Diff;

			Count++;
		}
	}

	if (Count > 0)
	{
		Sum /= Count;

		return Sum;
	}

	return FVector::ZeroVector;
}

void UMyFlockSteering::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = FMath::Clamp(DeltaTime * TurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * TimeScale;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}
		}
	}
	
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	
	FVector FlockDir = GetFlockDir();

	FVector NewDelta;

	if(!FlockDir.IsNearlyZero())
	{
		NewDelta = (FlockDir + ControlAcceleration) / 2.f;
	}
	else
	{
		NewDelta = ControlAcceleration;	
	}
	
	Velocity += NewDelta * FMath::Abs(Acceleration) * DeltaTime;
	//둘의 차이가 너무크니까 지터링되는것
	
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

	ConsumeInputVector();
}

void UMyFlockSteering::NotifyBumpedPawn(APawn* BumpedPawn)
{
	
}
