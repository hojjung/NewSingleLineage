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
	TArray<FName> m_AryEqupSlots;

	bool m_bIsRange;
	
protected:
	void EquipOption(const FName& itemWant);

	void UnequipOption(const FName& itemWant);

	void SetIsRangeStance();

public:
	void Init();

	bool Equip(EEquipSlotType slotWant,const FName& itemWant);

	void Unequip(EEquipSlotType slotWant);

	void Unequip(const FName& itemWant);

 	const TArray<FName>& GetEquipAry() const;

	bool IsItemEquipped(const FName& itemWant);

	bool IsItemEquipped(EEquipSlotType wantSlot);

	const FName& GetEquipItem(EEquipSlotType wantSlot);

	bool IsRangeStance();

	UParticleSystem* GetBulletEffect();
	
	void UpdateEquip();
};
