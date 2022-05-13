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

bool UCraftManager::TryCraft()
{
	if(!m_CrntItemData)
	{
		return false;
	}
	//재료 체크
	// bool IsMatrialEnough = IsMaterialEnough();
	//
	// if(!IsMatrialEnough)
	// {
	// 	PRINTF("UCraftManager::No MatrialEnough");
	// 	return false;
	// }
	// //공간체크
	// bool IsInvHasSpace = IsInvenHasSpace();
	//
	// if(!IsInvHasSpace)
	// {
	// 	PRINTF("UCraftManager::No InvenSpace");
	// 	return false;
	// }
	//
	// PurchaseItemForCraft();
		
	ReceiveItem();

	return true;
}

int UCraftManager::GetCraftAvailableCountWithMaterial()
{
	int MinCount = 10;

	for(const FCraftItemCost& Cost : m_CrntItemData->m_ItemData->m_AryCostItem)
	{
		// int InvenAmount = UMyLib::GetPlayerInven()->GetItemStack(Cost.m_ItemDataRowHandle.RowName);
		//
		// int StorageAmount = 0;
		//
		// for(UInventory* Inven : UMyLib::GetPlayerStorage())
		// {
		// 	StorageAmount += Inven->GetItemStack(Cost.m_ItemDataRowHandle.RowName);
		// }
		//
		// int HasAmount = InvenAmount + StorageAmount; 
		//
		// int Count = Cost.m_nStackOrLevel * GetAmount();
		//
		// int MaxCount = HasAmount / Count;
		//
		// if(MaxCount<MinCount)
		// {
		// 	MinCount = MaxCount; 
		// }
	}

	return MinCount;
}

int UCraftManager::GetCraftAvailableCountWithStackSize()
{
	int InvenStackAvailable = 10;
	
	// if(UMyLib::GetItemType(*m_CrntItemData) != EItemType::Equip)
	// {
	// 	int CurrentAmount = 0;//UMyLib::GetPlayerInven()->GetItemStack(m_CrntID);
	//
	// 	InvenStackAvailable = 10 - CurrentAmount;
	// }//스택 아이템의 경우 더 스택할수 있는지?
	return InvenStackAvailable;
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

bool UCraftManager::IsInvenHasSpace()
{
	// int Amount = GetAmount();
	//
	// if(UMyLib::IsEquip(*m_CrntItemData))
	// {
	// 	return UMyLib::GetPlayerInven()->IsCountAvailable(Amount);//소모품개수는,제작 개수를 결정할때 클램핑해주자.이함수는 제작후 남은 공간에 원하는 아이템을 넣을수 있는가
	// }
	//
	// return  Amount <= UMyGameInstance::Get->m_Inven->GetAvailalbeStackCount(m_CrntID);
	return true;
}

bool UCraftManager::IsMaterialEnough()
{
	for(const FCraftItemCost& Cost : m_CrntItemData->m_ItemData->m_AryCostItem)
	{
		if (UMyLib::IsEquip(Cost.m_ItemDataRowHandle.RowName))
		{
			if(!UMyLib::FindEquipItem(Cost.m_ItemDataRowHandle.RowName,Cost.m_nStackOrLevel))
			{
				return false;
			}
		}
		else
		{
			int Count = 1;//Cost.m_nStackOrLevel * GetAmount();
		
			if(!UMyLib::FindMiscItem(Cost.m_ItemDataRowHandle.RowName,Count))
			{
				return false;
			}	
		}
	}
	return true;
}

void UCraftManager::PurchaseItemForCraft()
{
	for(const FCraftItemCost& Cost : m_CrntItemData->m_ItemData->m_AryCostItem)
	{
		if (UMyLib::IsEquip(Cost.m_ItemDataRowHandle.RowName))
		{
			const FName* gidItem;
			UInventory* Inven = UMyLib::FindEquipItem(Cost.m_ItemDataRowHandle.RowName,Cost.m_nStackOrLevel,&gidItem);
			//Inven->RemoveEquipItem(*gidItem);
		}
		else
		{
			int Count = 1;//Cost.m_nStackOrLevel * GetAmount();

			UMyLib::RemoveMiscItem(Cost.m_ItemDataRowHandle.RowName,Count);
		}
	}
}

void UCraftManager::ReceiveItem()
{
	// if(m_OnCraft.IsBound())
	// {
	// 	m_OnCraft.Broadcast(m_CrntID); //퀘스트임
	// }

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

