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

protected:
	bool m_bIsRange;
	
	FItemSpec m_AryEqupSlots [(int)EEquipSlotType::Length];

protected:
	TQueue<TStrongObjectPtr<UEE_Base>> m_AryEqupEffects[(int)EEquipSlotType::Length];
	
protected:
	UPROPERTY()
	UInventory* m_BagInven;
	UPROPERTY()
	UInventory* m_BeltSlots;

	
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

	bool Unequip(EEquipSlotType slotWant, UInventory* returnInven , int * returnInvenIndex = nullptr);

	bool IsItemEquipped(EEquipSlotType wantSlot);

	const FItemSpec& GetEquipItem(int slotIndex);

	bool IsRangeStance();

	UParticleSystem* GetBulletEffect();

	void UpdateEquip();
	
public:
	void EquipBag(int i);
	
	void UnequipBag();

	void EquipBelt(int i);
	
	void UnequipBelt();

	bool IsBagUnequipable();

	bool IsBeltUnequipable();
	
	UInventory* GetBag() ;

	UInventory* GetBelt() ;

	UInventory::FOnInvenChanged& GetOnBagChanged();

	UInventory::FOnInvenChanged& GetOnBeltChanged();
};
