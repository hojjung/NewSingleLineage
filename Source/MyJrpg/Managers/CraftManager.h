// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "MyJrpg/DataTables/ItemData.h"
#include "UObject/NoExportTypes.h"
#include "CraftManager.generated.h"

/**
 * 만들수 있는 아이템 존재
 * 
 */
UCLASS()
class MYJRPG_API UCraftManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCraft,FText);

	FOnCraft m_OnCraft;
	

protected:
	UPROPERTY()
	int m_nCraftItemCount;
	UPROPERTY()
	int m_nIndex;
	TArray<FName> m_AryCraftDataKey;
	
	TArray<const FItemDataRow*> m_AryCraftDataRow;
	
protected:
	bool IsInvenHasSpace();

	bool IsGoldEnough();

	bool IsMaterialEnough();

	void PurchaseItemForCraft();

	void ReceiveItem();

	int GetCraftAvailableCountWithGold();

	int GetCraftAvailableCountWithMaterial();

	int GetCraftAvailableCountWithStackSize();
	
public:
	void InitLoadAllCraftItemData();
	
	void SetCraftItem(int index);

	void Clear();
	
	bool TryCraft();

	void SetCraftAmount(int v);

	int GetTotalCost();

public:
	FORCEINLINE const FItemDataRow& GetCrntItemRow() const
	{
		return *m_AryCraftDataRow[m_nIndex];
	}

	FORCEINLINE const FName& GetCrntItemKey() const
	{
		return m_AryCraftDataKey[m_nIndex];
	}

	FORCEINLINE const TArray<const FItemDataRow*>& GetAllCraftData() const
	{
		return m_AryCraftDataRow;
	}

	FORCEINLINE int GetAmount() const;
	
	int GetMaxAmount();
};
