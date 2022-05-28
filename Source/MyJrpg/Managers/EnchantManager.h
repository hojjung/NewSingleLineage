#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
#include "UObject/NoExportTypes.h"
#include "EnchantManager.generated.h"

class UInventory;
UCLASS()
class MYJRPG_API UEnchantManager : public UObject
{
	GENERATED_BODY()
	
public:
	UEnchantManager();

	DECLARE_MULTICAST_DELEGATE(FOnEnchantChangd);
	
	FOnEnchantChangd m_OnEnchantChanged;

protected:
	float m_AryWeaponPer[20];

	float m_AryArmorPer[20];

	float m_AryTrinketPer[20];

protected:
	TWeakObjectPtr<UInventory> m_InvenTarget;

	TWeakObjectPtr<UInventory> m_InvenMat;
	
	FItemSpec* m_TargetItem;

	FItemSpec* m_TargetMat;
	
protected:
	void EnchantSuccess(bool isSpecial);

	void EnchantFail();

	bool TryEnchant();

public:
	int GetCrntLevel() const;

	float GetEnchantPercent() const;
	
	void SetTargetEquip(FItemSpec& target, UInventory* inven);

	void SetMaterialEquip(FItemSpec& mat, UInventory* inven);

	const FItemSpec* GetTargetItem() const;

	const FItemSpec* GetTargetMat() const;

	bool IsAbleTarget(const FItemSpec& target);

	bool IsAbleMaterial(const FItemSpec& material);

	void Clear();
	
	void DoEnchant();

	bool IsEnchantAvailable() const;
};
