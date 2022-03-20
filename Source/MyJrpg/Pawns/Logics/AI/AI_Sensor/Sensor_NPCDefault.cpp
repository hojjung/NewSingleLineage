#include "Sensor_NPCDefault.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Team.h"
#include "Perception/PawnSensingComponent.h"

USensor_NPCDefault::USensor_NPCDefault()
{
	m_SightRadius = 900.f;
}

void USensor_NPCDefault::Init(ACombatUnitPawn* owner)
{
	Super::Init(owner);
}

void USensor_NPCDefault::UpdateAISensing()
{
	if (m_Owner->GetFocusedTarget())
	{
		return;
	}
	ACombatUnitPawn* Pawn = GetSensedPawn();

	if (Pawn)
	{
		if (m_Owner->GetFocusedTarget() == Pawn)
		{
			return;
		}

		m_Owner->SetFocusedTarget(Pawn);
	}
	else //안보일땐 이게되야함
	{
		if (m_Owner->GetFocusedTarget())
		{
			{
				m_Owner->SetFocusedTarget(nullptr); //FSM에서 OnFOcusLike,OnFOcusHate	
			}
		}
	}
}

bool USensor_NPCDefault::CheckDistAndAngle(const ABaseUnitPawn* Other)
{
	if (!Other)
	{
		return false;
	}

	FVector const OtherLoc = Other->GetActorLocation();

	FVector const SensorLoc = GetSensorLocation();

	FVector const SelfToOther = OtherLoc - SensorLoc;

	m_fCurrentTargetDist = SelfToOther.SizeSquared();

	if (m_fCurrentTargetDist > FMath::Square(m_SightRadius))
	{
		return false;
	}

	return true;
}

ACombatUnitPawn* USensor_NPCDefault::GetSensedPawn()
{
	AMyPlayerPawn* Player = UMyLib::GetPlayer();

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
