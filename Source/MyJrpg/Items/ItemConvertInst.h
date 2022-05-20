// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "MyJrpg/DataTables/ItemConvertTable.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"
#include "UObject/NoExportTypes.h"
#include "ItemConvertInst.generated.h"

/**
 *3 
 */



UCLASS()
class MYJRPG_API UItemConvertInst : public UInventory
{
	GENERATED_BODY()

public:
	enum EItemConvertIndex
	{
		Left,
		Right,
		Fuel,
		Cost
	};

private:
	const FItemConvertRow* m_ItemConvertRow;

	const FItemConvertSet* m_SelectedConvertSet;
	
	float m_fConvertTimer;

	float m_fMaxConvertTimer;

	float m_fFireTimer;

	float m_fMaxFireTimer;
	
private:
	virtual void Init(int size) override;

	void OnInvenChanged();
	
	bool CheckLeftItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckCostItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckFuelItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckRightItemEmpty();

public:
	void SetConvertData(const FItemConvertRow& convertRow);
	
	const FItemConvertRow& GetConvertRow() const;
	
	const FItemSpec& GetLeftItem() const;

	const FItemSpec& GetRightItem() const;
	
	const FItemSpec& GetFuelItem() const;

	const FItemSpec& GetCostItem() const;
	
	bool CheckLeftItemAvailable(const FItemSpec& item);

	bool CheckFuelItemAvailable(const FItemSpec& item);

	bool CheckCostItemAvailable(const FItemSpec& item);
	
	void Tick(float delta_time);
	
	bool IsEmpty();

	float GetRemainTimePer();

	float GetFireRemainTimePer();
};



