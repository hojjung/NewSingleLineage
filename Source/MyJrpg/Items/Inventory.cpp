#include "Inventory.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"


UInventory::UInventory()
{

}

void UInventory::Init(int size)
{
	m_nInvenMaxSize = size;
	
	m_AryItems.Reserve(m_nInvenMaxSize);

	m_AryItems.Init(FItemSpec(), m_nInvenMaxSize);	
}

int UInventory::GetTotalItemCount()
{
	return m_AryItems.Num();
}

bool UInventory::CheckEmptySlot(int needCount)
{
	int RemainSlotCount = m_nInvenMaxSize - (GetEmptyIndex());
	
	return RemainSlotCount >= needCount; //엠티 인덱스가 전체 사이즈 -1 이라면 더이상 칸이 없는거임
}

bool UInventory::CheckEmptyStack(FName id, int needCount)
{
	FItemSpec* ItemSpec = GetItem(id);

	if(!ItemSpec)
	{
		return false;
	}

	if(ItemSpec->m_nStack + needCount > FGlobalVariable::INVEN_MAXSTACK)
	{
		return false;
	}

	return true;
}

int UInventory::GetStackableCount(FName id)
{
	FItemSpec* ItemSpec = GetItem(id);

	if(!ItemSpec)
	{
		return FGlobalVariable::INVEN_MAXSTACK;
	}

	return FGlobalVariable::INVEN_MAXSTACK - ItemSpec->m_nStack; 
}

bool UInventory::IsItemWillBeZeroOnErase(FName id, int amount)
{
	int CurrentAmount = GetItemAmount(id);

	int Remain = CurrentAmount - amount; 

	return Remain <= 0;
}

FItemSpec* UInventory::GetItem(FName id)
{
	int Index = GetItemIndex(id);

	if(Index<=INDEX_NONE)
	{
		return nullptr;
	}

	return &m_AryItems[Index]; 
}

int UInventory::GetItemIndex(FName id)
{
	int Index = 0;

	for (const FItemSpec& Item : m_AryItems)
	{
		if (Item.m_ItemID == id)
		{
			return Index;
		}

		Index++;
	}

	return INDEX_NONE;
}



int UInventory::GetEmptyIndex()
{
	int Index = 0;

	for (const FItemSpec& Item : m_AryItems)
	{
		if (Item.IsEmpty())
		{
			break;
		}

		Index++;
	}

	return Index;
}

UInventory::EResult UInventory::AddItem(FName id, int& amount, int level)
{
	const FItemDataRow* FoundItemRow = &UMyLib::GetItemData(id);

	check(FoundItemRow);

	EItemType ItemType = UMyLib::GetItemType(id);

	if (ItemType == EItemType::Equip)
	{
		if (!CheckEmptySlot(amount)) //템꽉참
		{
			return EResult::FailMaxCount;
		}

		FItemSpec NewItem(id, 1, level);

		int Iter = 0;

		while (Iter < amount)
		{
			AddItemInst(NewItem);

			Iter++;
		}

		m_OnItemObtain.Broadcast(*FoundItemRow,Iter);
		
		return EResult::Success;
	}
	else
	{
		FItemSpec* ItemFound = GetItem(id);
		
		if (ItemFound) //이미 가진거면
		{
			if(ItemFound->m_nStack + amount > FGlobalVariable::INVEN_MAXSTACK)
			{
				return EResult::FailStackCount;//일단 더해주고 남는건 남겨야한다.
			}

			ItemFound->m_nStack += amount;
		}
		else
		{
			if (!CheckEmptySlot(1))
			{
				return EResult::FailMaxCount;
			}
			//인덱스 한개씩 빗나가서 이상한게 등록되고있다.
			FItemSpec NewItem(id, amount, level);

			int Index = GetEmptyIndex();

			m_AryItems[Index] = NewItem;

			m_OnNewItemAdded.Broadcast(id);
		}
	}

	m_OnInvenChanged.Broadcast();

	m_OnItemObtain.Broadcast(*FoundItemRow,amount);

	return EResult::Success;
}

UInventory::EResult UInventory::AddItemInst(const FItemSpec& itemOld) //복사해서씀
{
	if (!CheckEmptySlot(1)) //템꽉참
	{
		return EResult::FailMaxCount;
	}

	int Index = GetEmptyIndex();

	m_AryItems[Index] = itemOld; //일단 넣어주고

	m_OnNewItemAdded.Broadcast(itemOld.m_ItemID); //배열 자체를 새로만들어서 맵에 등록
		
	m_OnInvenChanged.Broadcast();
	//
	
	return EResult::Success;
}

int UInventory::RemoveItem(FName id, int amount)
{
	UInventory::EResult Result = EResult::Success;

	EItemType ItemType = UMyLib::GetItemType(id);

	int IndexFound = GetItemIndex(id);

	if(IndexFound==INDEX_NONE)
	{
		return -amount;
	}

	FItemSpec* FoundItem = &m_AryItems[IndexFound];

	FoundItem->m_nStack -= amount;

	int OverAmount = 0;
		
	if (FoundItem->m_nStack <= 0)
	{
		OverAmount = FoundItem->m_nStack;
		
		m_AryItems.RemoveAt(IndexFound); //배열에서 먼저지워주기
		
		m_AryItems.Add(FItemSpec()); //지웠으니까 슈링크 되고 새로운 빈아이템 추가

		m_OnNewItemRemoved.Broadcast(id);
	}

	m_OnInvenChanged.Broadcast();

	return OverAmount;
}

void UInventory::RemoveItem(const FItemSpec& itemOld)
{
	bool Found = false;

	int Index = 0;

	for (; Index < m_AryItems.Num(); Index++)
	{
		const FItemSpec& Item = m_AryItems[Index];

		if (Item == itemOld)
		{
			Found = true;

			break;
		}
	}

	if (Found)
	{
		m_AryItems.RemoveAt(Index);

		m_AryItems.Add(FItemSpec());

		m_OnInvenChanged.Broadcast();
	}
}

void UInventory::LevelUpEquipItem(const FItemSpec& itemOld)
{
	int32 Index = m_AryItems.Find(itemOld);

	m_AryItems[Index].m_nLevel++;
}

int UInventory::GetItemAmount(FName id)
{
	FItemSpec* FoundItemSpec = GetItem(id);
	
	return  FoundItemSpec ? FoundItemSpec->m_nStack : 0;
}

bool UInventory::IsInvenHasSpace(const FItemDataRow& crafting_data,int amount)
{
	int  EraseCount=0;
	
	for (const FCraftItemCost& Cost : crafting_data.m_AryCostItem)
	{
		if (IsItemWillBeZeroOnErase(Cost.m_ItemDataRowHandle.RowName, Cost.m_nStackCount*amount))
		{
			++EraseCount;
		}
	}

	int LackCount = amount - EraseCount;//추가 필요칸 4개, 지워질칸 3개,1개 더 비워야함//추가 필요칸 2개,지워질칸 10개

	if(LackCount<=0)
	{
		return true;
	}
	
	if (CheckEmptySlot(LackCount))
	{
		return true; 
	}

	return false;
}

bool UInventory::CheckHasItem(FName key, int count)
{
	return GetItemAmount(key) >= count;
}

const FItemSpec& UInventory::GetItem(int index) const
{
	return m_AryItems[index];
}
