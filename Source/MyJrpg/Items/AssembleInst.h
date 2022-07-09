// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "AssembleInst.generated.h"

/**
 *  아이템 추가
 *  크래프트 에이블에서 가져오는값
 *  각 칸이 슬롯이 되어준다.
 */
UCLASS()
class MYJRPG_API UAssembleInst : public UInventory
{
	GENERATED_BODY()

private:
	const FCraftable* m_Craftable;

public:
	void SetCraftItem(const FCraftable& craftable);

	bool IsSlotPutable(int index, const FItemSpec& item);

	bool IsSlotMatch(int index);

	const FCraftable& GetCraftData() const;

	bool TryComplete();

	void PutItem(int index, const FItemSpec& item);
};
