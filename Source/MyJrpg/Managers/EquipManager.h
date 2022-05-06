// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
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
	UPROPERTY()
	UInventory* m_Inven;
	
	TArray<FItemSpec*> m_AryEqupSlots;

	bool m_bIsRange;
	
protected:
	void EquipOption(const FName& itemWant);

	void UnequipOption(const FName& itemWant);

	void SetIsRangeStance();

public:
	void Init();

 	FORCEINLINE const TArray<FItemSpec*>& GetEquipAry() const
 	{
 		return m_AryEqupSlots;
 	}
	void Equip(EEquipSlotType slotWant, int invenIndex);

	void Unequip(EEquipSlotType slotWant);

	void Unequip(const FName& itemWant);

	bool IsItemEquipped(EEquipSlotType t, const FItemSpec& itemWant);

	bool IsItemEquipped(const FItemSpec& itemWant);

	bool IsItemEquipped(EEquipSlotType wantSlot);

	const FItemSpec* GetEquipItem(int slotIndex);

	bool IsRangeStance();

	UParticleSystem* GetBulletEffect();
	
	void UpdateEquip();
};
