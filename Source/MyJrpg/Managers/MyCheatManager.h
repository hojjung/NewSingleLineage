// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MyCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMyCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	UFUNCTION(Exec)
	void AddItemToPlayer(FName id,int amount);
	UFUNCTION(Exec)
	void RemoveItemFromPlayer(FName id,int amount);

	UFUNCTION(Exec)
	void RemoveItemFromFocusedTarget(FName id,int amount);

	UFUNCTION(Exec)
	void AddTestItemToPlayer();

	UFUNCTION(Exec)
	void AddQuest(FName id);

	UFUNCTION(Exec)
	void TryCompleteQuest(FName id);
};
