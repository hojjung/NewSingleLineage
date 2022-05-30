#include "EquipManager.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/EquipEffect/EE_Base.h"

void UEquipManager::Init()
{
	int Iter = -1;
	
	int Len = (int)EEquipSlotType::Length;
	
	while (++Iter < Len)
		m_AryEqupSlots[Iter] = FItemSpec();
}

void UEquipManager::Equip(EEquipSlotType slotWant,UInventory* inven, int invenIndex)
{
	FItemSpec Item = inven->GetItemRef(invenIndex);
	
	inven->ClearSlot(invenIndex);
	
	Unequip(slotWant,inven,&invenIndex,false);
	
	int SlotIndex = (int)slotWant;
	
	m_AryEqupSlots[SlotIndex] = Item;

	EquipOption(SlotIndex, m_AryEqupSlots[SlotIndex]);
	
	SetIsRangeStance();
	
	m_OnEquipChanged.Broadcast();
}

void UEquipManager::EquipOption(int index, const FItemSpec& itemWant)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(itemWant.m_ID);
	const TArray<TSubclassOf<UEE_Base>>& AryEffects = ItemData.m_EquipEffects;
	if (AryEffects.Num() > 0)
	{
		for (TSubclassOf<UEE_Base> EE : AryEffects)
		{
			TStrongObjectPtr<UEE_Base> StrongEE(NewObject<UEE_Base>(this, EE));

			m_AryEqupEffects[index].Enqueue(StrongEE);

			StrongEE->Equip();
		}
	}
	//UMyGameInstance::Get->m_PlayerStatManager->EquipItem(itemWant.m_ID);
}

void UEquipManager::UnequipOption(int index, const FItemSpec& itemWant)
{
	TStrongObjectPtr<UEE_Base> StrongEE;
	
	while(m_AryEqupEffects[index].Dequeue(StrongEE))
	{
		StrongEE->UnEquip();
	}
	m_AryEqupEffects[index].Empty();
	//UMyGameInstance::Get->m_PlayerStatManager->UnequipItem(itemWant.m_ID);
}

bool UEquipManager::Unequip(EEquipSlotType slotWant, UInventory* returnInven , int * returnInvenIndex, bool updateDele)
{
	PRINTF("Unequip01");
	int SlotIndex = (int)slotWant;

	FItemSpec Temp = m_AryEqupSlots[SlotIndex];
	if(Temp.m_ID.IsNone())
	{
		return true;
	}

	UnequipOption(SlotIndex,m_AryEqupSlots[SlotIndex]);
	m_AryEqupSlots[SlotIndex] = FItemSpec();

	if(!returnInvenIndex)
	{
		if(!returnInven->AddItem(Temp))
		{
			return false;
		}
	}
	else
	{
		returnInven->AddSlot(*returnInvenIndex,Temp);//이거때문에 위 if문을 합치면 안된다
	}
	
	SetIsRangeStance();
	
	if(updateDele)
	{
		m_OnEquipChanged.Broadcast();
	}

	return true;
}

void UEquipManager::DestoryItem(EEquipSlotType t)
{
	GetEquipItem(t) = FItemSpec();

	SetIsRangeStance();
	m_OnEquipChanged.Broadcast();
}

bool UEquipManager::IsItemEquipped(EEquipSlotType wantSlot)
{
	return !GetEquipItem((int)wantSlot).m_ID.IsNone();
}

bool UEquipManager::IsItemEquipped(const FItemSpec& itemWant)
{
	const FItemDataRow& Data = UMyLib::GetItemData(itemWant.m_ID);
	
	if(!UMyLib::IsEquip(Data))
	{
		return false;
	}
	
	if(!IsItemEquipped(Data.m_ItemType))
	{
		return false;
	}

	return &GetEquipItem(Data.m_ItemType) == &itemWant;
}

FItemSpec& UEquipManager::GetEquipItem(int slotIndex)
{
	return m_AryEqupSlots[slotIndex];
}

FItemSpec& UEquipManager::GetEquipItem(EEquipSlotType t)
{
	return GetEquipItem((int)t);
}

bool UEquipManager::IsRangeStance()
{
	return m_bIsRange;
}

void UEquipManager::SetIsRangeStance()
{
	const auto& FoundItem = GetEquipItem((int)EEquipSlotType::Weapon);

	if(FoundItem.m_ID.IsNone())
	{
		m_bIsRange = false;
		return ;
	}
	
	m_bIsRange = UMyLib::GetItemData(FoundItem.m_ID).m_WeaponData.m_bIsRange;
}

UParticleSystem* UEquipManager::GetBulletEffect()
{
	const auto& FoundItem = GetEquipItem((int)EEquipSlotType::Weapon);
	
	if(FoundItem.m_ID.IsNone())
	{
		m_bIsRange = false;
		return nullptr;
	}

	const FColorDataRow* ColorData = UMyLib::GetItemData(FoundItem.m_ID).m_ColorHandle.GetRow<FColorDataRow>("");

	return ColorData->m_Bullet;
}

void UEquipManager::UpdateEquip()
{
	m_OnEquipChanged.Broadcast();
}
///////////////////////

///////////////////////
void UEquipManager::EquipBag(int i)
{
	m_BagInven = NewObject<UInventory>(this);

	m_BagInven->Init(i);

	m_BagInven->UpdateInventory();
}

void UEquipManager::UnequipBag()
{
	m_BagInven = nullptr;
}

void UEquipManager::EquipBelt(int i)
{
	m_BeltSlots	= NewObject<UInventory>(this);

	m_BeltSlots->Init(i);

	m_BeltSlots->UpdateInventory();
}

void UEquipManager::UnequipBelt()
{
	m_BeltSlots = nullptr;
}

bool UEquipManager::IsBagUnequipable()
{
	if(!GetBag())
	{
		return true;
	}
	return m_BagInven->GetUsingSlotCount() <= 0;	
}

bool UEquipManager::IsBeltUnequipable()
{
	if(!GetBelt())
	{
		return true;
	}
	return m_BeltSlots->GetUsingSlotCount() <= 0;
}

UInventory* UEquipManager::GetBag() 
{
	return m_BagInven;
}

UInventory* UEquipManager::GetBelt() 
{
	return m_BeltSlots;
}

UInventory** UEquipManager::GetBeltHolder()
{
	return &m_BeltSlots;
}

UInventory::FOnInvenChanged& UEquipManager::GetOnBagChanged()
{
	return GetBag()->GetOnInvenChanged();
}

UInventory::FOnInvenChanged& UEquipManager::GetOnBeltChanged()
{
	return GetBelt()->GetOnInvenChanged();
}

void UEquipManager::ReduceDurability(EEquipSlotType t, int amount)
{
	if(GetEquipItem(t).m_ID.IsNone())
	{
		return;
	}
	GetEquipItem(t).m_nDurability -= amount;

	UpdateDur();
	
	if(GetEquipItem(t).m_nDurability < 1)
	{
		DestoryItem(t);
	}
}

void UEquipManager::UpdateDur()
{
	m_OnDurChanged.Broadcast();
}

bool UEquipManager::HasSpace(FItemSpec& addItem)
{
	bool IsEquip = UMyLib::IsEquip(addItem.m_ID);

	if(IsEquip)
	{
		if(UMyLib::GetPlayerInven()->EmptySlotCount() > 0)
		{
			return true;
		}
		if(GetBag() && GetBag()->EmptySlotCount() > 0)
		{
			return true;
		}
		if(GetBelt() && GetBelt()->EmptySlotCount() > 0)
		{
			return true;
		}
		return false;
	}

	if(UMyLib::GetPlayerInven()->HasSpace(addItem))
	{
		return true;
	}
	if(GetBag() && GetBag()->HasSpace(addItem))
	{
		return true;
	}
	if(GetBelt() && GetBelt()->HasSpace(addItem))
	{
		return true;
	}
	
	return false;
}

bool UEquipManager::HasSpace(FItemSpec&& addItem)
{
	return HasSpace(addItem);
}

bool UEquipManager::AddItem(FItemSpec& addItem, bool newEquipItem)
{
	if(UMyLib::GetPlayerInven()->AddItem(addItem,newEquipItem))
	{
		return true;
	}
	else
	{
		if(GetBag() && GetBag()->AddItem(addItem,newEquipItem))
		{
			return true;
		}
		else
		{
			if(GetBelt() && GetBelt()->AddItem(addItem,newEquipItem))
			{
				return true;
			}	
		}
	}
	
	return false;
}

bool UEquipManager::AddItem(FItemSpec&& addItem, bool newEquipItem)
{
	return  AddItem(addItem, newEquipItem);
}

FItemSpec* UEquipManager::FindItemInEquip(FName id)
{
	int Iter = -1;
	while (++Iter < (int)EEquipSlotType::Length)
	{
		if(GetEquipItem(Iter).m_ID == id)
		{
			return &GetEquipItem(Iter);
		}
	}
	return nullptr;
}
