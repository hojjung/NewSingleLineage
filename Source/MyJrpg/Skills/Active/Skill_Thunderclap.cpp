#include "Skill_Thunderclap.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USkill_Thunderclap::OnSkillTriggered()
{
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits,0,m_SkillData->m_fRadius))
	{
		return;
	}

	for(AActor* Hit : Hits)
	{
		AMonsterPawn* Mobs = Cast<AMonsterPawn>(Hit);

		Mobs->TakeDmg(GetDamage(),m_Player);
	}
}
