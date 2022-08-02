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
	static float Dest;

	static float Sep;

	static float Align;
public:
	UFUNCTION(Exec)
	void AddItemToPlayer(FName id,int amount);
	UFUNCTION(Exec)
	void RemoveItemFromPlayer(FName id,int amount);

	UFUNCTION(Exec)
	void RemoveItemFromFocusedTarget(FName id,int amount);

	UFUNCTION(Exec)
	void AddQuest(FName id);

	UFUNCTION(Exec)
	void TryCompleteQuest(FName id);
	UFUNCTION(Exec)
	void KillPlayer();
	UFUNCTION(Exec)
	void SetDest(float v);
	UFUNCTION(Exec)
	void SetAlign(float v);
	UFUNCTION(Exec)
	void SetSep(float v);
	UFUNCTION(Exec)
	void SetClear();
};
