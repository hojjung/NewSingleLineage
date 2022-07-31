#include "MyFlockSteering.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MonsterPawn.h"


void UMyFlockSteering::BeginPlay()
{
	Super::BeginPlay();
	m_SetIgnoreSelf.Reset();
	m_SetIgnoreSelf.Add(m_Owner);
	m_NearMobs.Reserve(20);
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

	FVector NewDelta;
	
	if(m_Owner->GetFocusedTarget<>())
	{
		m_NearMobs.Reset();
		UMyGameInstance::Get->m_ZoneInst->GetNearNpcs<ACombatUnitPawn>(m_Owner,m_NearMobs,400);
		
		FVector cohesionVec = ControlAcceleration * 1.2f;
		FVector alignmentVec = GetFlockDir();
		FVector separationVec = CalculateSeparationVector();
		
		NewDelta = cohesionVec + alignmentVec + separationVec;
		NewDelta = NewDelta.GetSafeNormal();
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

FVector UMyFlockSteering::GetFlockDir()
{
	FVector Sum =  m_Owner->GetActorForwardVector();

	if(m_NearMobs.Num() <= 0)
	{
		return Sum;	
	}

	for (ACombatUnitPawn* OtherActor : m_NearMobs)
	{
		Sum += OtherActor->GetActorForwardVector();
	}
	Sum /= m_NearMobs.Num();
		
	return Sum.GetSafeNormal();
}

FVector UMyFlockSteering::CalculateSeparationVector()
{
	FVector Sum = FVector::ZeroVector;
	
	if(m_NearMobs.Num() <= 0)
	{
		return Sum;	
	}

	for (ACombatUnitPawn* OtherActor : m_NearMobs)
	{
		Sum += (m_Owner->GetActorLocation() - OtherActor->GetActorLocation());
	}
	Sum /= m_NearMobs.Num();
		
	return Sum.GetSafeNormal();
}