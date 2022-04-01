#pragma once

#include "CoreMinimal.h"
#include "GameRuleBase.h"
#include "Rule_Default.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API URule_Default : public UGameRuleBase
{
	GENERATED_BODY()

public:
	virtual void Init(const FZone& zone) override;

	virtual void OnMonsterDead(AMonsterPawn* monster_pawn) override;
};
