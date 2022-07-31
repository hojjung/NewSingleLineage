#include "Sensor_NPCDefault.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Team.h"
#include "Perception/PawnSensingComponent.h"

void USensor_NPCDefault::Init(ACombatUnitPawn* owner)
{
	Super::Init(owner);
}

void USensor_NPCDefault::UpdateAISensing()
{
	ACombatUnitPawn* FoundPawn = m_Owner->GetFocusedTarget<ACombatUnitPawn>();
	
	if (UMyGameInstance::Get->m_TeamKarma->IsFoe(m_Owner) && FoundPawn && FoundPawn->IsAlive())
	{
		return;//영원한 추격? 그만 쫓아오는 로직도 있어야하지 않나?
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

bool USensor_NPCDefault::CheckDistAndAngle(const ACombatUnitPawn* Other)
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

	if(!Other->IsSneak())//은신을 안했다며
	{
		return true;//찾을수있다?
	}
	
	FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();

	FVector const MyFacingDir = GetSensorRotation().Vector();
	
	return (SelfToOtherDir | MyFacingDir) >= m_PeripheralVisionCosine;
}