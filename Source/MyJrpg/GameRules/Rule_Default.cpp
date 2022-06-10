#include "Rule_Default.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void URule_Default::OnMonsterDead(AMonsterPawn* monster_pawn)
{
	UMyGameInstance::Get->m_RewardManager->OnMonsterDead(monster_pawn);
}
