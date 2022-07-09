#include "CraftManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/Build/BuildInteract/BI_Assemble.h"
#include "MyJrpg/Items/Inventory.h"

void UCraftManager::Init()
{
	Clear();

	for(const auto& it : UBuildData::GetBuildTable->GetRowMap())
	{
		const FBuildDataRow* Build = (FBuildDataRow*)it.Value;
		
		if(Build->m_BuildType == EBuildType::Foundation || Build->m_BuildType == EBuildType::Wall
			|| Build->m_AryCostItem.Num() < 1 || !Build->m_bShowInPanel)
		{
			continue;
		}
		m_AryCraftables.Emplace(FCraftDataInfo(it.Key,false));
	}
	
	m_AryCraftables.Sort([](const FCraftDataInfo& LHS, const FCraftDataInfo& RHS)
	{
		if(LHS.m_ItemData->m_nCraftLevelLimit == RHS.m_ItemData->m_nCraftLevelLimit)
		{
			return LHS.m_ItemData->m_nCraftSortOrder < RHS.m_ItemData->m_nCraftSortOrder;	
		}
		return LHS.m_ItemData->m_nCraftLevelLimit < RHS.m_ItemData->m_nCraftLevelLimit;
	});
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
	if(!UMyLib::IsTestMode())
	{
		PurchaseItemForCraft(m_CrntItemData->m_ItemData->m_AryCostItem);
	}
		
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
		UMyLib::PrintErrorText(NSLOCTEXT("UCraftManager","EmptyData","선택된 아이템 없음").ToString());
		return false;
	}
	if(UMyLib::IsTestMode())
	{
		return true;
	}
	//재료 체크
	bool IsMatrialEnough = IsMaterialEnough(m_CrntItemData->m_ItemData->m_AryCostItem);
	
	if(!IsMatrialEnough)
	{
		UMyLib::PrintErrorText(NSLOCTEXT("UCraftManager","Lack of Material","재료 부족").ToString());
		return false;
	}
	//공간체크
	bool IsInvHasSpace = IsInvenHasSpace();
	
	if(!IsInvHasSpace)
	{
		UMyLib::PrintErrorText(NSLOCTEXT("UCraftManager","No Inven Space","가방 공간없음").ToString());
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
		return  UMyLib::HasSpaceAllInven(Item);
	}
	return true;
}

bool UCraftManager::IsMaterialEnough(const TArray<FCraftItemCost>& aryCraftCost)
{
	if(UMyLib::IsTestMode())
	{
		return true;
	}
	for(const FCraftItemCost& Cost : aryCraftCost)
	{
		if(!UMyLib::FindItemAllInven(Cost.m_ItemDataRowHandle.RowName, Cost.m_nStackOrLevel))
		{
			return false;
		}
	}
	return true;
}

void UCraftManager::PurchaseItemForCraft(const TArray<FCraftItemCost>& aryCraftCost)
{
	for(const FCraftItemCost& Cost : aryCraftCost)
	{
		UMyLib::RemoveItemAll(Cost.m_ItemDataRowHandle.RowName, Cost.m_nStackOrLevel);
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
		UMyLib::AddItemAll(Items,true);
	}
	else
	{
		UMyLib::GetBuildManager()->AddFurniture(m_CrntItemData->m_ID);
	}
}

