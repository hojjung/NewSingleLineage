#include "Sensor_LogicBase.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Team.h"
#include "Perception/PawnSensingComponent.h"

USensor_LogicBase::USensor_LogicBase()
{
	m_SensingInterval = 0.3f;

	SetPeripheralVisionAngle(22);
}

void USensor_LogicBase::Init(ACombatUnitPawn* owner)
{
	m_Owner=owner;

	m_SightRadius = owner->GetSightRange();
	
	SetSensingUpdatesEnabled(true);
}

void USensor_LogicBase::SetSensingUpdatesEnabled(const bool bEnabled)
{
	if (bEnabled && m_SensingInterval > 0.f)
	{
		const float InitialDelay = (m_SensingInterval * FMath::SRand()) + KINDA_SMALL_NUMBER;
		SetTimer(InitialDelay);
	}
	else
	{
		SetTimer(0.f);
	}
}


void USensor_LogicBase::SetTimer(const float TimeInterval)
{
	if (m_Owner && GEngine->GetNetMode(GetWorld()) < NM_Client)
	{
		m_Owner->GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &USensor_LogicBase::OnTimer,
		                                         TimeInterval,
		                                         false);
	}
}

void USensor_LogicBase::OnTimer()
{
	if (!m_Owner || !m_Owner->GetWorld())
	{
		return;
	}

	UpdateAISensing();

	SetTimer(m_SensingInterval);
};

void USensor_LogicBase::SetSensingInterval(const float NewSensingInterval)
{
	if (m_SensingInterval != NewSensingInterval)
	{
		m_SensingInterval = NewSensingInterval;

		if (m_Owner)
		{
			if (m_SensingInterval <= 0.f)
			{
				SetTimer(0.f);
			}
			else
			{
				float CurrentElapsed = m_Owner->GetWorldTimerManager().GetTimerElapsed(TimerHandle_OnTimer);

				CurrentElapsed = FMath::Max(0.f, CurrentElapsed);

				if (CurrentElapsed < m_SensingInterval)
				{
					SetTimer(m_SensingInterval - CurrentElapsed);
				}
				else if (CurrentElapsed > m_SensingInterval)
				{
					SetTimer(KINDA_SMALL_NUMBER);
				}
			}
		}
	}
}

bool USensor_LogicBase::HasLineOfSightTo(const AActor* Other) const
{
	return m_Owner->LineOfSightTo(Other);
}

void USensor_LogicBase::UpdateAISensing()
{
	
}

FVector USensor_LogicBase::GetSensorLocation() const
{
	FVector SensorLocation(FVector::ZeroVector);
	const AActor* SensorActor = GetSensorActor();

	if (SensorActor)
	{
		FRotator ViewRotation;
		SensorActor->GetActorEyesViewPoint(SensorLocation, ViewRotation);
	}

	return SensorLocation;
}

FRotator USensor_LogicBase::GetSensorRotation() const
{
	FRotator SensorRotation(FRotator::ZeroRotator);

	const AActor* SensorActor = GetSensorActor();

	if (SensorActor)
	{
		SensorRotation = SensorActor->GetActorRotation();
	}

	return SensorRotation;
}


bool USensor_LogicBase::CheckIsHidden(APawn* Pawn) const
{
	return Pawn->IsHidden();
}

AActor* USensor_LogicBase::GetSensorActor() const
{
	return m_Owner;
}

bool USensor_LogicBase::IsSensorActor(const AActor* Actor) const
{
	return (Actor == GetSensorActor());
}

bool USensor_LogicBase::CheckDistAndAngle(const ACombatUnitPawn* Other)
{
	if (!Other)
	{
		return false;
	}

	FVector const OtherLoc = Other->GetActorLocation();
	
	FVector const SensorLoc = GetSensorLocation();
	
	FVector const SelfToOther = OtherLoc - SensorLoc;

	float const SelfToOtherDistSquared = SelfToOther.SizeSquared();
	
	if (SelfToOtherDistSquared > FMath::Square(m_SightRadius))
	{
		return false;
	}
	
	FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();

	FVector const MyFacingDir = GetSensorRotation().Vector();
	
	return (SelfToOtherDir | MyFacingDir) >= m_PeripheralVisionCosine;
}

void USensor_LogicBase::SetPeripheralVisionAngle(const float NewPeripheralVisionAngle)
{
	m_PeripheralVisionAngle = NewPeripheralVisionAngle;
	
	m_PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(m_PeripheralVisionAngle));
}

ACombatUnitPawn* USensor_LogicBase::GetSensedPawn()
{
	AMyPlayerPawn* Player = UMyLib::GetPlayer();

	if(!Player->IsAlive())
	{
		return nullptr;
	}
	
	if (!HasLineOfSightTo(Player))
	{
		return nullptr;
	}

	if (CheckIsHidden(Player))
	{
		return nullptr;
	}

	if (!CheckDistAndAngle(Player)) //스텔스보다 먼저해야함
		{
		return nullptr;
		}

	return Player;
}
