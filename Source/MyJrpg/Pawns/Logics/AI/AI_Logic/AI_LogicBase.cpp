#include "AI_LogicBase.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void UAI_LogicBase::Init(ACombatUnitPawn* owner)
{
	m_Owner = owner;
};

void UAI_LogicBase::Tick(float delta)
{
};

bool UAI_LogicBase::CheckTargetRange(float rangeSqr)
{
	float DistSqr = FVector::DistSquared(m_Owner->GetActorLocation(), m_Owner->GetFocusedActorLocation());

	return DistSqr <= rangeSqr;
}

bool UAI_LogicBase::CheckAngle(float angleEuler)
{
	return UMyLib::CheckAngle(m_Owner,Cast<AActor>(m_Owner->GetFocusedTarget()),angleEuler);
}

EKarma UAI_LogicBase::GetUnitKarma(const ACombatUnitPawn* Other)
{
	EKarma Karma = UMyGameInstance::Get->m_TeamKarma->GetUnitKarma(Other);
	
	return  Karma;
}

void UAI_LogicBase::OnTargetFocused(const ACombatUnitPawn* Other)
{
	switch (GetUnitKarma(Other))
	{
	case EKarma::Neutral:
		OnFocusNeutral();
		break;
	case EKarma::Friendly:
		OnFocusFriendly();
		break;
	case EKarma::Hate:
		OnFocusHate();
		break;
	}
}

void UAI_LogicBase::OnFocusNeutral()
{
	
}

void UAI_LogicBase::OnFocusFriendly()
{
	
}

void UAI_LogicBase::OnFocusHate()
{
	
}
