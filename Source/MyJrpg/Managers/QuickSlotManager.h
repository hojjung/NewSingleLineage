// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Items/Inventory.h"
#include "UObject/NoExportTypes.h"
#include "QuickSlotManager.generated.h"

class UItemExecuteBase;
/**
 *한슬롯에 여러개가 존재한다면?
 */

UCLASS()
class MYJRPG_API UQuickSlotManager : public UObject
{
	GENERATED_BODY()

protected:
	TMap<TSubclassOf<UItemExecuteBase>, TStrongObjectPtr<UItemExecuteBase>> m_MapItemExe;

	UPROPERTY()
	UInventory* m_QuickItem;

public:
	void RegisterItem(TSubclassOf<UItemExecuteBase> exe);

	void UnregisterItem(TSubclassOf<UItemExecuteBase> exe);

	int ExeItem(TSubclassOf<UItemExecuteBase> exe,UInventory* inven, int index, int cnt);

	int ExeItem(TSubclassOf<UItemExecuteBase> exe,UInventory* inven, FItemSpec& item, int cnt);
};
