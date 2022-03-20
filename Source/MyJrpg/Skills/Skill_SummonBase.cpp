#include "Skill_SummonBase.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USkill_SummonBase::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	
	SpawnUnit();
}

bool USkill_SummonBase::SpawnUnit()
{
	FNavLocation Loc;
	
	if(!UMyLib::GetNavSys()->GetRandomReachablePointInRadius(m_Player->GetActorLocation(),m_SkillData->m_fRadius,Loc))
	{
		return false;
	}

	if(!UMyGameInstance::Get->m_SpawnManager->SummonUnit(m_SummonUnitId,Loc,m_SkillData->m_fDuration))
	{
		return false;
	}

	return true;
}
