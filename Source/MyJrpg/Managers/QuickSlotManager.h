// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Items/Item_Exe/ItemExecuteBase.h"
#include "UObject/NoExportTypes.h"
#include "QuickSlotManager.generated.h"

/**
 *한슬롯에 여러개가 존재한다면?
 */

USTRUCT()
struct FQuickCount
{
	GENERATED_BODY()

public:
	TStrongObjectPtr<UItemExecuteBase> m_ItemExe;

	int m_nAmount;

	FQuickCount()
	{
		m_nAmount = 0;
	}
	FQuickCount(TSubclassOf<UItemExecuteBase> newExeClass, UObject* outer)
	{
		m_ItemExe = TStrongObjectPtr<UItemExecuteBase>(NewObject<UItemExecuteBase>(outer, newExeClass));

		m_nAmount = 1;
	}
};
UCLASS()
class MYJRPG_API UQuickSlotManager : public UObject
{
	GENERATED_BODY()

protected:
	TMap<TSubclassOf<UItemExecuteBase>, FQuickCount> m_MapItemExe;

	UPROPERTY()
	UInventory* m_QuickItem;

protected:
	void RegisterItem(TSubclassOf<UItemExecuteBase> exe);

	void UnregisterItem(TSubclassOf<UItemExecuteBase> exe);

public:
	void RegisterQuickItemExe(const FItemDataRow& itemData);
	
	void UnregisterQuickItemExe(const FItemDataRow& itemData);
	
	int ExeItem(TSubclassOf<UItemExecuteBase> exe,UInventory* inven, int index, int cnt);

	int ExeItem(TSubclassOf<UItemExecuteBase> exe,UInventory* inven, FItemSpec& item, int cnt);
};
