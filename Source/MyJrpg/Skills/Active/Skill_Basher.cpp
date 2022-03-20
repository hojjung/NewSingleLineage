// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Basher.h"

#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USkill_Basher::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	TArray<AActor*> Hits;
	if(!TraceBox(Hits,50,m_SkillData->m_BoxHalfSize))
	{
		return;
	}
	for(AActor* Mob : Hits)
	{
		Cast<AMonsterPawn>(Mob)->TakeDmg(GetDamage(),m_Player);
	}
}
