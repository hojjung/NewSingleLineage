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


struct FCraftDataInfo
{
public:
	FCraftDataInfo(): m_IsItem(false), m_ItemData(nullptr)
	{
	}

	FCraftDataInfo(FName id, bool isItem)
	{
		m_ID = id;

		m_IsItem = isItem;
		
		m_ItemData = m_IsItem ? UItemData::GetItemTable->FindRow<FCraftable>(m_ID,"") : UBuildData::GetBuildTable->FindRow<FCraftable>(m_ID,"");

		
	}
public:
	bool m_IsItem;//or furniture
	
	FName m_ID;

	const FCraftable* m_ItemData;
};

UCLASS()
class MYJRPG_API UCraftManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCraft,const FName&);

	FOnCraft m_OnCraft;
	
protected:
	TArray<FCraftDataInfo> m_AryCraftables;

	const FCraftDataInfo* m_CrntItemData;
	
protected:
	bool IsInvenHasSpace();

	void ReceiveItem();

public:
	void Init();
	
	void PurchaseItemForCraft(const TArray<FCraftItemCost>& aryCraftCost);
	
	bool IsMaterialEnough(const TArray<FCraftItemCost>& aryCraftCost);
	
	void SetCraftItem(int index);

	void Clear();

	bool CheckCraftable();
	
	void Craft();

	const FCraftDataInfo* GetCrntItemRow() const;

	const TArray<FCraftDataInfo>& GetAryCraftables() const;
	
	void AddCraftItemData(FName id);
};
