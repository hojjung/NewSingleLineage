// Fill out your copyright notice in the Description page of Project Settings.


#include "Sensor_ForgetTarget.h"

#include "MyJrpg/MyLib.h"


void USensor_ForgetTarget::Init(ACombatUnitPawn* owner)
{
	Super::Init(owner);
}

void USensor_ForgetTarget::UpdateAISensing()
{
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
		if (m_Owner->GetFocusedTarget())
		{
			m_Owner->SetFocusedTarget(nullptr); //FSM에서 OnFOcusLike,OnFOcusHate	
		}
}

bool USensor_ForgetTarget::CheckDistAndAngle(const ACombatUnitPawn* Other)
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
}//원래 앞으로 가면 안되고,(은신을해도걸림?,뒤로가도 걸린다. 즉 각도 계산 없이 사거리 연산
//근데 은신을 할경우, 앞을 의미하는 각도를 초과하기 전까진 걸리지 않는다.

ACombatUnitPawn* USensor_ForgetTarget::GetSensedPawn()
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
