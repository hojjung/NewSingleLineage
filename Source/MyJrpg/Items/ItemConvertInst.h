// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "MyJrpg/DataTables/ItemConvertTable.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"
#include "UObject/NoExportTypes.h"
#include "ItemConvertInst.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UItemConvertInst : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FChanged);

	FChanged m_OnChanged;
	
private:
	const FItemConvertRow* m_ItemConvertRow;

	const FItemConvertSet* m_SelectedConvertSet;
	
	FItemSpec m_LeftItem;

	FItemSpec m_RightItem;

	FItemSpec m_CostItem;

	float m_fTimer;

private:
	const FItemConvertSet* TryStartConvert();

	bool CheckLeftItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckCostItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckRightItemEmpty();

public:
	void SetConvertData(const FItemConvertRow& convertRow);
	
	const FItemConvertRow& GetConvertRow() const;
	
	const FItemSpec& GetLeftItem() const;

	const FItemSpec& GetRightItem() const;
	
	const FItemSpec& GetCostItem() const;
	
	bool CheckLeftItemAvailable(const FItemSpec& item);

	bool CheckCostItemAvailable(const FItemSpec& item);
	
	void SetLeftItem(UInventory* fromInven, int fromIndex);

	void SetCostItem(UInventory* fromInven, int fromIndex);

	void StartConvert();
	
	void Tick(float delta_time);
	
	bool IsEmpty();
	
	void OnDropItem(UInventory* get, int m_n_index);
};



