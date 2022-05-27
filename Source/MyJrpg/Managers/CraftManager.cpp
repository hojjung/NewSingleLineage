#include "CraftManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"

void UCraftManager::Init()
{
	Clear();

	for(auto& it : UBuildData::GetBuildTable->GetRowMap())
	{
		FBuildDataRow* Build = (FBuildDataRow*)it.Value;
		
		if(Build->m_BuildType == EBuildType::Foundation || Build->m_BuildType == EBuildType::Wall || Build->m_AryCostItem.Num() < 1)
		{
			continue;
		}
		m_AryCraftables.Emplace(FCraftDataInfo(it.Key,false));
	}
	
	m_AryCraftables.Sort([](const FCraftDataInfo& LHS, const FCraftDataInfo& RHS)  { return LHS.m_ItemData->m_nCraftLevelLimit < RHS.m_ItemData->m_nCraftLevelLimit; });
}

void UCraftManager::AddCraftItemData(FName id)
{
	m_AryCraftables.Emplace(FCraftDataInfo(id, true));
}

void UCraftManager::SetCraftItem(int index)
{
	m_CrntItemData = &m_AryCraftables[index];
}

void UCraftManager::Craft()
{
	PurchaseItemForCraft();
		
	ReceiveItem();
}

const FCraftDataInfo* UCraftManager::GetCrntItemRow() const
{
	return m_CrntItemData;
}

const TArray<FCraftDataInfo>& UCraftManager::GetAryCraftables() const
{
	return m_AryCraftables;
}

void UCraftManager::Clear()
{
	m_CrntItemData = nullptr;
}

bool UCraftManager::CheckCraftable()
{
	if(!m_CrntItemData)
	{
		return false;
	}
	//재료 체크
	bool IsMatrialEnough = IsMaterialEnough();
	
	if(!IsMatrialEnough)
	{
		PRINTF("UCraftManager::No MatrialEnough");
		return false;
	}
	//공간체크
	bool IsInvHasSpace = IsInvenHasSpace();
	
	if(!IsInvHasSpace)
	{
		PRINTF("UCraftManager::No InvenSpace");
		return false;
	}
	return true;
}

bool UCraftManager::IsInvenHasSpace()
{
	FItemSpec Item;
	
	Item.m_ID = m_CrntItemData->m_ID;
	
	Item.m_nLvStack = 1;
	
	if(m_CrntItemData->m_IsItem)
	{
		return  UMyLib::GetPlayerInven()->HasSpace(Item);
	}
	//UMyLib::GetBuildManager()->AddFurniture(m_CrntItemData->m_ID);
	return true;
}

bool UCraftManager::IsMaterialEnough()
{
	for(const FCraftItemCost& Cost : m_CrntItemData->m_ItemData->m_AryCostItem)
	{
		if(!UMyLib::GetPlayerInven()->FindItem(Cost.m_ItemDataRowHandle.RowName, Cost.m_nStackOrLevel))
		{
			return false;
		}
	}
	return true;
}

void UCraftManager::PurchaseItemForCraft()
{
	for(const FCraftItemCost& Cost : m_CrntItemData->m_ItemData->m_AryCostItem)
	{
		UMyLib::GetPlayerInven()->RemoveItem(Cost.m_ItemDataRowHandle.RowName, Cost.m_nStackOrLevel);
	}
}

void UCraftManager::ReceiveItem()
{
	if(m_OnCraft.IsBound())
	{
		m_OnCraft.Broadcast(m_CrntItemData->m_ID);
	}

	if(m_CrntItemData->m_IsItem)
	{
		FItemSpec Items(m_CrntItemData->m_ID,UMyLib::IsEquip(*((FItemDataRow*)(m_CrntItemData->m_ItemData))) ? 0 : 1);
		UMyLib::GetPlayerInven()->AddItem(Items);
	}
	else
	{
		UMyLib::GetBuildManager()->AddFurniture(m_CrntItemData->m_ID);
	}
}

