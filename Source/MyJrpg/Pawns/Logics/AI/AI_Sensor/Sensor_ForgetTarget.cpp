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
}