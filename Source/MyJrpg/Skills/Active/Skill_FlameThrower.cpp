#include "Skill_FlameThrower.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USkill_FlameThrower::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	TArray<AActor*> Hits;
	if(!TraceSphere(Hits, m_SkillData->m_fRange, m_SkillData->m_fRadius))
	{
		return;
	}
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		Monster->TakeDmg(GetDamage(), m_Player);
	}
}
