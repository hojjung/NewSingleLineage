// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/BuildData.h"

#include "MyJrpg/DataTables/ItemData.h"
#include "UObject/NoExportTypes.h"
#include "CraftManager.generated.h"

/**
 * 만들수 있는 아이템 존재
 * 
 */


UENUM(BlueprintType)
enum class ECraftType :uint8
{
	Item,
	Furniture
};

USTRUCT()
struct FCraftable
{
	GENERATED_BODY()

public:
	FCraftable(): m_Row(nullptr), m_TypeCraft()
	{
		
	}

	FCraftable(const FName& id,unsigned char* r, ECraftType t)
	{
		m_ID = id;
		
		m_Row = r;

		m_TypeCraft = t;
	}

public:
	FName m_ID;
	
	unsigned char* m_Row;
	
	ECraftType m_TypeCraft;

	int GetLimitLevel() const
	{
		switch (m_TypeCraft)
		{
		case ECraftType::Item:
			return ((FItemDataRow*)m_Row)->m_nCraftLevelLimit;
		case ECraftType::Furniture:
			return ((FBuildDataRow*)m_Row)->m_nCraftLevelLimit;
		}
		return -1;
	}

	const TArray<FCraftItemCost>& GetAryCraftCosts() const
	{
		switch (m_TypeCraft)
		{
		case ECraftType::Item:
			return ((FItemDataRow*)m_Row)->m_AryCostItem;
		}
		return ((FBuildDataRow*)m_Row)->m_AryCostItem;
	}

	const FEntityRow* GetEntityRow() const
	{
		return ((FEntityRow*)m_Row);
	}
};
UCLASS()
class MYJRPG_API UCraftManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCraft,const FName&);

	FOnCraft m_OnCraft;
	
protected:
	TArray<FCraftable> m_AryCraftables;
	
	UPROPERTY()
	int m_nCraftItemCount;

	const FCraftable* m_CrntItemData;
	
protected:
	bool IsInvenHasSpace();

	bool IsMaterialEnough();

	void PurchaseItemForCraft();

	void ReceiveItem();

	int GetCraftAvailableCountWithMaterial();

	int GetCraftAvailableCountWithStackSize();
	
public:
	void Init();
	
	void SetCraftItem(int index);

	void Clear();
	
	bool TryCraft();

	void SetCraftAmount(int v);

	int GetAmount();
	
	int GetMaxAmount();

public:
	const FCraftable* GetCrntItemRow() const;

	const TArray<FCraftable>& GetAryCraftables() const;
	
	void AddCraftItemData(FName id, const FItemDataRow* element);
};
