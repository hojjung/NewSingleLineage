// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_DrillAttack.h"

#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USkill_DrillAttack::OnSkillTriggered()//트리거를 여러개 놓기
{
	Super::OnSkillTriggered();

	if(!m_Player->GetFocusedTarget())
	{
		return;
	}
	m_Player->GetFocusedTarget<ACombatUnitPawn>()->TakeDmg(GetDamage(),m_Player);
}
