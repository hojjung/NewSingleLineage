// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Items/Inventory.h"
#include "UObject/NoExportTypes.h"
#include "QuickSlotManager.generated.h"

/**
 * 칸개수가 줄었다가 늘었다가
 * 벨트를 장착했다가 탈착했다가
 * 벨트에 뭐가 잇으면 탈착이 안됨
 *
 * 벨트에 드래그 드랍된것은, 큇슬롯 위젯이 사용시켜줌
 */
UCLASS()
class MYJRPG_API UQuickSlotManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnInvenChanged);

	FOnInvenChanged m_OnBagChanged;

	FOnInvenChanged m_OnBeltChanged;
	
protected:
	UPROPERTY()
	UInventory* m_BagInven;
	UPROPERTY()
	UInventory* m_BeltSlots;
	
public:
	bool EquipQuickSlot(int cnt);

	bool TryUnequipQuickSlot();

	bool EquipBag(int cnt);

	bool TryUnequipBag();

	const UInventory* GetInven() const;

	const UInventory* GetSlot() const;
};
