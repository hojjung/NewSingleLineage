#include "Skill_SummonDemons.h"

USkill_SummonDemons::USkill_SummonDemons(const FObjectInitializer& obj):Super(obj)
{
	m_SummonUnitId = TEXT("Demon02");
}

void USkill_SummonDemons::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	int iter = 0;
	
	while (iter < 4)
	{
		SpawnUnit();
		iter++;
	}
}
