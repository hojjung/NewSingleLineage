#include "CollectionManager.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"

void UCollectionManager::Init()
{
	m_AryItemKeys = UItemCollectionTable::GetItemCollecTable->GetRowNames();
	
	UItemCollectionTable::GetItemCollecTable->GetAllRows<FItemCollecRow>("",m_AryItemRows);

	int Iter = 0;
	
	for(const auto& Key : m_AryItemKeys)
	{
		int Count = m_AryItemRows[Iter]->m_AryItems.Num();
		TUnlockedItems Items;
		m_MapCollecSpec.Emplace(Key,Items).Init(false,Count);

		TSubclassOf<UOptionBase> Op = m_AryItemRows[Iter]->m_ClassOption;

		if(m_MapTotalStats.Contains(Op))
		{
			m_MapTotalStats[Op][1]++;
		}
		else
		{
			TOptionGroups& Ary = m_MapTotalStats.Emplace(Op, TOptionGroups());
			Ary.Init(0,2);
			Ary[1] = 1;//index 1 is the Total, 0 is the current
		}

		Iter++;
	}

	CalculateComplete();
}

int UCollectionManager::GetTotalCount()
{
	return m_AryItemRows.Num();
}

void UCollectionManager::CalculateComplete()
{
	m_nCompleteCount = 0;

	for(const TTuple<FName, TUnlockedItems>& Pair : m_MapCollecSpec)
	{
		for(bool Had : Pair.Value)
		{
			if (Had)
			{
				m_nCompleteCount++;

				auto OpKey = UItemCollectionTable::GetItemCollecTable->FindRow<FItemCollecRow>(Pair.Key,"")->m_ClassOption;

				m_MapTotalStats[OpKey][0]++;
			}
		}
	}
}

int UCollectionManager::GetCompleteCount()
{
	return m_nCompleteCount;
}

const TArray<FItemCollecRow*>& UCollectionManager::GetAryCollec() const
{
	return m_AryItemRows;
}

const TArray<FName>& UCollectionManager::GetAryCollecKeys() const
{
	return m_AryItemKeys;
}

const TMap<FName, TArray<bool>>& UCollectionManager::GetMapCollecSpecs() const
{
	return m_MapCollecSpec;
}

bool UCollectionManager::IsItemRegistered(FName collectionID, int indexItem)
{
	return m_MapCollecSpec[collectionID][indexItem];
}

const TMap<TSubclassOf<UOptionBase>, UCollectionManager::TOptionGroups>& UCollectionManager::GetMapOptionGroups() const
{
	return m_MapTotalStats;
}

FText UCollectionManager::GetTotalProgressText()
{
	FString Str = FString::Printf(TEXT("%d/%d"),GetCompleteCount(),GetTotalCount());

	return FText::FromString(Str);
}

FText UCollectionManager::GetCollecProgressText(FName collectionID)
{
	int CompleCount = 0;
	
	TArray<bool>& ArySpec = m_MapCollecSpec[collectionID];

	for(bool Unlocked : ArySpec)
	{
		if (Unlocked)
			CompleCount++;
	}
	
	FString Str = FString::Printf(TEXT("%d/%d"), CompleCount, ArySpec.Num());

	return FText::FromString(Str);
}

float UCollectionManager::GetTotalProgress()
{
	int Total = GetTotalCount(); 
	if(Total <= 0)
	{
		return 0.f;
	}
	return (float)GetCompleteCount() / (float)Total;	
}

bool UCollectionManager::CheckCanAdd(FName collectionID, int indexItem, UInventory* invenFrom)
{
	const FItemCollecRow& ItemRow = *UItemCollectionTable::GetItemCollecTable->FindRow<FItemCollecRow>(collectionID,"");

	const FItemCollecNeed& WantAdd = ItemRow.m_AryItems[indexItem];

	bool IsEquip = UMyLib::GetItemType(WantAdd.m_Item.RowName) == EItemType::Equip;

	// if (IsEquip)
	// {
	// 	const FName* FoundEquipItemGId = invenFrom->FindEquipItem(WantAdd.m_Item.RowName, WantAdd.m_nEnchantLv);
	//
	// 	if(!FoundEquipItemGId)
	// 	{
	// 		return false;
	// 	}
	// }
	// else
	// {
	// 	if(!invenFrom->FindMisItem(WantAdd.m_Item.RowName, 1))
	// 	{
	// 		return false;
	// 	}
	// }

	return true;
}

void UCollectionManager::UpdateCompleteCollec(FName collectionID, int indexItem, const FItemCollecRow& ItemRow)
{
	auto& AryItems = m_MapCollecSpec[collectionID];
	
	AryItems[indexItem] = true;

	int Sum = 0;
	
	for(auto Ele : AryItems)
	{
		if(Ele)
		{
			Sum++;
		}
	}

	if(Sum >= AryItems.Num())
	{
		m_nCompleteCount++;
		m_MapTotalStats[ItemRow.m_ClassOption][0]++;
	}
}

void UCollectionManager::AddItem(FName collectionID, int indexItem)
{
	const FItemCollecRow& ItemRow = *UItemCollectionTable::GetItemCollecTable->FindRow<FItemCollecRow>(collectionID,"");

	const FItemCollecNeed& WantAdd = ItemRow.m_AryItems[indexItem];

	bool IsEquip = UMyLib::GetItemType(WantAdd.m_Item.RowName) == EItemType::Equip;

	UInventory* InvenFrom = nullptr;

	if(IsEquip)
	{
		const FName* FoundItem = nullptr;
		
		InvenFrom = UMyLib::FindEquipItem(WantAdd.m_Item.RowName, WantAdd.m_nEnchantLv,&FoundItem);
		
		//InvenFrom->RemoveEquipItem(*FoundItem);
	}
	else
	{
		InvenFrom = UMyLib::FindMiscItem(WantAdd.m_Item.RowName);
		InvenFrom->RemoveItem(WantAdd.m_Item.RowName, 1);
	}
	//
	UpdateCompleteCollec(collectionID, indexItem, ItemRow);

	m_OnCollecChanged.Broadcast();
}
