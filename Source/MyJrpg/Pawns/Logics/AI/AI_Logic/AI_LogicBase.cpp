#include "AI_LogicBase.h"

#include "MyJrpg/MyLib.h"
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
	return UMyLib::CheckAngle(m_Owner,m_Owner->GetFocusedTarget(),angleEuler);
}
