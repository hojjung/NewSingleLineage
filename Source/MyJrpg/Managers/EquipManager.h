// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/MyJrpg.h"
//#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Items/EquipEffect/EE_Base.h"
#include "UObject/NoExportTypes.h"
#include "EquipManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UEquipManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnEquipChanged);

	FOnEquipChanged m_OnEquipChanged;

	FOnEquipChanged m_OnDurChanged;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemEarn, const FItemDataRow& item, int amount);

	FOnItemEarn m_OnItemEarn;

protected:
	bool m_bIsRange;
	
	FItemSpec m_AryEqupSlots [(int)EEquipSlotType::Length];

protected:
	TQueue<TStrongObjectPtr<UEE_Base>> m_AryEqupEffects[(int)EEquipSlotType::Length];
	
protected:
	UPROPERTY()
	UInventory* m_BagInven;
	
protected:
	void EquipOption(int index, const FItemSpec& itemWant);

	void UnequipOption(int index, const FItemSpec& itemWant);

	void SetIsRangeStance();

public:
 	FORCEINLINE const FItemSpec* GetEquipAry() const
 	{
 		return m_AryEqupSlots;
 	}

	void Init();

	void Equip(EEquipSlotType slotWant, UInventory* inven, int invenIndex);

	bool Unequip(EEquipSlotType slotWant, UInventory* returnInven , int * returnInvenIndex = nullptr, bool updateDele = true);

	void DestoryItem(EEquipSlotType t);

	bool IsItemEquipped(EEquipSlotType wantSlot);

	bool IsItemEquipped(const FItemSpec& itemWant);

	FItemSpec& GetEquipItem(int slotIndex);

	FItemSpec& GetEquipItem(EEquipSlotType t);

	bool IsRangeStance();

	UParticleSystem* GetBulletEffect();

	void UpdateEquip();
	
public:
	void EquipBag(int i);
	
	void UnequipBag();

	bool IsBagUnequipable();

	UInventory* GetBag() ;

	UInventory::FOnInvenChanged& GetOnBagChanged();

	void ReduceDurability(EEquipSlotType t, int amount);

	void UpdateDur();
	
public:
	bool HasSpace(const FItemSpec& addItem);

	bool HasSpace(const FItemSpec&& addItem);

	bool AddItem(FItemSpec& addItem, bool newEquipItem = false);

	bool AddItem(FItemSpec&& addItem, bool newEquipItem = false);
	
	FItemSpec* FindItemInEquip(FName id);

public:
	void ClearEquipSlots();

	void ClearAllEquipment();
	
	void TakeDurDmg(int am);
};
