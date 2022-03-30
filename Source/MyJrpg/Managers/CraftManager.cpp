#include "CraftManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"

void UCraftManager::InitLoadAllCraftItemData()
{
	TArray<FItemDataRow*> TempAryRow;
	UItemData::GetItemTable->GetAllRows("",TempAryRow);
	
	TArray<FName> TempAryKey = UItemData::GetItemTable->GetRowNames();

	int Iter = 0;
	
	for(const FItemDataRow* CraftData : TempAryRow)
	{
		if(CraftData->m_AryCostItem.Num() > 0)
		{
			m_AryCraftDataKey.Add(TempAryKey[Iter]);
			m_AryCraftDataRow.Add(CraftData);
		}
		Iter++;
	}

	SetCraftAmount(1);

	m_nIndex = INDEX_NONE;
}

void UCraftManager::SetCraftItem(int index)
{
	m_nIndex = index;
}

bool UCraftManager::TryCraft()
{
	if(m_nIndex == INDEX_NONE)
	{
		return false;
	}
	
	bool IsGldEnough = IsGoldEnough();

	if(!IsGldEnough)
	{
		PRINTF("UCraftManager::No Gold");
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
	
	PurchaseItemForCraft();
		
	ReceiveItem();

	return true;
}

void UCraftManager::SetCraftAmount(int v)
{
	m_nCraftItemCount = v;
}

int UCraftManager::GetTotalCost()
{
	return GetCrntItemRow().m_nCraftCost * GetAmount();
}

int UCraftManager::GetAmount() const
{
	return m_nCraftItemCount;
}

int UCraftManager::GetCraftAvailableCountWithGold()
{
	if(GetCrntItemRow().m_nCraftCost<=0)
	{
		return FGlobalVariable::INVEN_MAXSTACK;
	}
	
	return UMyGameInstance::Get->m_CurrencyManager->GetGold() / GetCrntItemRow().m_nCraftCost * GetAmount(); 
}//1000원 소유 = 가격 200원 = 5개, 가격 0원?

int UCraftManager::GetCraftAvailableCountWithMaterial()
{
	int MinCount = FGlobalVariable::INVEN_MAXSTACK;
	
	for(const FCraftItemCost& Cost : GetCrntItemRow().m_AryCostItem)
	{
		int InvenAmount = UMyLib::GetPlayerInven()->GetItemStack(Cost.m_ItemDataRowHandle.RowName);

		int StorageAmount = UMyLib::GetPlayerStorage()->GetItemStack(Cost.m_ItemDataRowHandle.RowName);

		int HasAmount = InvenAmount + StorageAmount; 
		
		int Count = Cost.m_nStackOrLevel * GetAmount();

		int MaxCount = HasAmount / Count;

		if(MaxCount<MinCount)
		{
			MinCount = MaxCount; 
		}
	}

	return MinCount;
}

int UCraftManager::GetCraftAvailableCountWithStackSize()
{
	int InvenStackAvailable = FGlobalVariable::INVEN_MAXSTACK;
	
	if(UMyLib::GetItemType(GetCrntItemKey()) != EItemType::Equip)
	{
		int CurrentAmount = UMyLib::GetPlayerInven()->GetItemStack(GetCrntItemKey());

		InvenStackAvailable = FGlobalVariable::INVEN_MAXSTACK - CurrentAmount;
	}//스택 아이템의 경우 더 스택할수 있는지?
	return InvenStackAvailable;
}

int UCraftManager::GetMaxAmount()
{
	//못사더라도 제작 가능횟수가 있음, 5만골 보유,아이템의 가격 7만원,=이경우 맥스는 1
	int GoldAvailable = GetCraftAvailableCountWithGold();

	int MaterialAvailable = GetCraftAvailableCountWithMaterial();

	int InvenStackAvailable = GetCraftAvailableCountWithStackSize();

	return FMath::Min3(GoldAvailable,MaterialAvailable,InvenStackAvailable);
}

void UCraftManager::Clear()
{
	SetCraftAmount(1);

	m_nIndex = INDEX_NONE;
}

bool UCraftManager::IsInvenHasSpace()
{
	int Amount = GetAmount();
	
	if(UMyLib::IsEquip(GetCrntItemKey()))
	{
		return UMyLib::GetPlayerInven()->IsCountAvailable(Amount);//소모품개수는,제작 개수를 결정할때 클램핑해주자.이함수는 제작후 남은 공간에 원하는 아이템을 넣을수 있는가
	}
	
	return  Amount <= UMyGameInstance::Get->m_Inven->GetAvailalbeStackCount(GetCrntItemKey());	
}

bool UCraftManager::IsGoldEnough()
{
	return UMyGameInstance::Get->m_CurrencyManager->CheckGoldEnough(GetCrntItemRow().m_nCraftCost * GetAmount());
}

bool UCraftManager::IsMaterialEnough()
{
	for(const FCraftItemCost& Cost : GetCrntItemRow().m_AryCostItem)
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
	UMyGameInstance::Get->m_CurrencyManager->SubGold(GetCrntItemRow().m_nCraftCost);

	for(const FCraftItemCost& Cost : GetCrntItemRow().m_AryCostItem)
	{
		if (UMyLib::IsEquip(Cost.m_ItemDataRowHandle.RowName))
		{
			const FName* gidItem;
			UInventory* Inven = UMyLib::FindEquipItem(Cost.m_ItemDataRowHandle.RowName,Cost.m_nStackOrLevel,&gidItem);
			Inven->RemoveEquipItem(*gidItem);
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
	if(m_OnCraft.IsBound())
	{
		//for quest
		m_OnCraft.Broadcast(GetCrntItemRow().m_TextShowingName);
	}

	if(!UMyLib::IsEquip(GetCrntItemKey()))
	{
		UMyLib::GetPlayerInven()->AddItem(GetCrntItemKey(),m_nCraftItemCount);
	}
	else
	{
		FName HashID = UMyLib::GenerateEquipItemHashKey(GetCrntItemKey(),this);

		UMyLib::GetPlayerInven()->AddEquipItem(HashID);
	}
	
}

