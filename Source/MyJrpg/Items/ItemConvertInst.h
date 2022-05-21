// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "MyJrpg/DataTables/ItemConvertTable.h"
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
	DECLARE_MULTICAST_DELEGATE(FOnInvenChanged);

	FOnInvenChanged m_OnConvertChanged;
	
	enum EItemConvertIndex
	{
		Left,
		Right,
		Fuel,
		Cost,
		Len
	};

private:
	const FItemConvertRow* m_ItemConvertRow;

	const FItemConvertSet* m_SelectedConvertSet;

	bool m_bAllMaterialAvailable;

	bool m_bIsFireWorking;

	bool m_bIsConverting;

	bool m_bIsNeedFire;

	float m_fConvertTimer;

	float m_fMaxConvertTimer;

	float m_fFireTimer;

	float m_fMaxFireTimer;

	FDateTime m_ConvertFinishTime;
	
private:
	virtual void Init(int size) override;

	void OnInvenChanged();
	
	bool CheckLeftItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckCostItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckFuelItemWithSet(const FItemConvertSet& set, const FItemSpec& item);

	bool CheckRightItemEmpty();

	void ReceiveRightItem();
	
	void RemoveItemStack(EItemConvertIndex t, int amount);
	
public:
	void SetConvertData(const FItemConvertRow& convertRow);
	void EndFireWorks();
	void EndConverts();
	void TryUseFuelToFire();
	void TrySelectItemConvertSet();
	void TryStartConvert();
	void CheckLeftOrCostAvailable();

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

	float GetRemainTime() const;

	bool IsFireWorking();

	bool IsConvertWorking();

	const FItemDataRow* GetRightItemData();
};



