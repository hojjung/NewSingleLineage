// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"
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
	TArray<const FItemSpec*> m_AryEqupSlots;

	bool m_bIsRange;
	
protected:
	void EquipOption(const FItemSpec& itemWant);

	void UnequipOption(const FItemSpec& itemWant);

	void SetIsRangeStance();

public:
	void Init();

	bool Equip(EEquipSlotType slotWant,const FItemSpec& itemWant);

	void Unequip(EEquipSlotType slotWant);

	void Unequip(const FItemSpec& itemWant);

	const TArray<const FItemSpec*>& GetEquipAry() const;

	bool IsItemEquipped(const FItemSpec& itemWant);

	bool IsItemEquipped(EEquipSlotType wantSlot);

	const FItemSpec* GetEquipItem(EEquipSlotType wantSlot);

	bool IsRangeStance();

	UParticleSystem* GetBulletEffect();
	
	void UpdateEquip();
};
