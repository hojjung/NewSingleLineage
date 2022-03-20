
#include "Skill_Whirlwind.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Pawns/Components/MyMovement.h"

void USkill_Whirlwind::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits,0,m_SkillData->m_fRadius))
	{
		return;
	}
	for(AActor* Mob : Hits)
	{
		Cast<AMonsterPawn>(Mob)->TakeDmg(GetDamage(),m_Player);
	}
}
bool USkill_Whirlwind::CanUseSkill()
{
	if(!m_Player)
	{
		m_Player =  UMyLib::GetPlayer();	
	}
	return CheckCooltime();
}
