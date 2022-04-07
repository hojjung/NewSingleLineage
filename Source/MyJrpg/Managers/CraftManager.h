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

	TMap<FName, const FCraftItemCost*> m_MapCraftingItems;

	FName m_CrntID;

	const FItemDataRow* m_CrntItemData;
	
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
	void Init();
	
	void SetCraftItem(const FName& id);

	void Clear();
	
	bool TryCraft();

	void SetCraftAmount(int v);

	int GetTotalCost();

	int GetAmount();
	
	int GetMaxAmount();

public:
	void AddCraftItemData(const FName& itemKey, const FCraftItemCost& craft);

	const TMap<FName, const FCraftItemCost*>& GetCraftItems() const
	{
		return m_MapCraftingItems;
	}

	const FItemDataRow* GetCrntItemRow() const;
};
