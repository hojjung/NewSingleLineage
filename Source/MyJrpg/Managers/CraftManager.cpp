#include "CraftManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"

void UCraftManager::Init()
{
	Clear();

	m_AryCraftables.Reset();
	
	for(auto& it : UBuildData::GetBuildTable->GetRowMap())
	{
		FBuildDataRow* Build = (FBuildDataRow*)it.Value;
		if(Build->m_BuildType != EBuildType::Furniture || Build->m_AryCostItem.Num() < 1)
		{
			continue;
		}
		m_AryCraftables.Emplace(FCraftable(it.Key, it.Value,ECraftType::Furniture));
	}
	
	m_AryCraftables.Sort([](const FCraftable& LHS, const FCraftable& RHS)  { return LHS.GetLimitLevel() > RHS.GetLimitLevel(); });
}

void UCraftManager::AddCraftItemData(FName id, const FItemDataRow* element)//아이템이 너무 많으니까 반복문 밖에서 한번돌려줌
{
	m_AryCraftables.Emplace(FCraftable(id, (unsigned char *)element,ECraftType::Item));
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

void UCraftManager::SetCraftAmount(int v)
{
	m_nCraftItemCount = v;
}

int UCraftManager::GetAmount()
{
	return m_nCraftItemCount;
}

int UCraftManager::GetCraftAvailableCountWithMaterial()
{
	int MinCount = 10;

	const TArray<FCraftItemCost>& AryItems = m_CrntItemData->GetAryCraftCosts();
	
	for(const FCraftItemCost& Cost : AryItems)
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

int UCraftManager::GetMaxAmount()
{
	int MaterialAvailable = GetCraftAvailableCountWithMaterial();

	int InvenStackAvailable = GetCraftAvailableCountWithStackSize();

	return FMath::Min(MaterialAvailable,InvenStackAvailable);
}

const FCraftable* UCraftManager::GetCrntItemRow() const
{
	return m_CrntItemData;
}

const TArray<FCraftable>& UCraftManager::GetAryCraftables() const
{
	return m_AryCraftables;
}


void UCraftManager::Clear()
{
	SetCraftAmount(1);

	//m_CrntID = NAME_None;

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
	const TArray<FCraftItemCost>& AryItems = m_CrntItemData->GetAryCraftCosts();
	
	for(const FCraftItemCost& Cost : AryItems)
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
			int Count = Cost.m_nStackOrLevel * GetAmount();
		
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
	const TArray<FCraftItemCost>& AryItems = m_CrntItemData->GetAryCraftCosts();
	
	for(const FCraftItemCost& Cost : AryItems)
	{
		if (UMyLib::IsEquip(Cost.m_ItemDataRowHandle.RowName))
		{
			const FName* gidItem;
			UInventory* Inven = UMyLib::FindEquipItem(Cost.m_ItemDataRowHandle.RowName,Cost.m_nStackOrLevel,&gidItem);
			//Inven->RemoveEquipItem(*gidItem);
		}
		else
		{
			int Count = Cost.m_nStackOrLevel * GetAmount();

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

	switch (m_CrntItemData->m_TypeCraft)
	{
	case ECraftType::Item:
		{
			FItemSpec Items(m_CrntItemData->m_ID,UMyLib::IsEquip(*((FItemDataRow*)(m_CrntItemData->m_Row))) ? 0 : 1);
			UMyLib::GetPlayerInven()->AddItem(Items);
		}
		break;
	case ECraftType::Furniture:
		{
			UMyLib::GetBuildManager()->AddFurniture(m_CrntItemData->m_ID);
		}
		break;
	}
}

