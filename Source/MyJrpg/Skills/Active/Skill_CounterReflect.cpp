#include "Skill_CounterReflect.h"

#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"


void USkill_CounterReflect::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	m_Player->TakeInvincible(m_SkillData->m_fDuration);
	m_DealHandle = UMyGameInstance::Get->m_PlayerStatManager->m_OnTookDmg.AddUObject(this, &USkill_CounterReflect::OnPlayerTookDmg);
}

void USkill_CounterReflect::EndSkillAnim()
{
	Super::EndSkillAnim();
	UMyGameInstance::Get->m_PlayerStatManager->m_OnPlayerHpChanged.Remove(m_DealHandle);

	float FinalDamage = (m_fDmgTotal * 2) + m_SkillData->m_fDamage;
	
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits, m_SkillData->m_fRange, m_SkillData->m_fRadius))
	{
		return;
	}
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		Monster->TakeDmg(FinalDamage, m_Player);
	}
}

void USkill_CounterReflect::OnPlayerTookDmg(float dmg)
{
	m_fDmgTotal += dmg;
}

