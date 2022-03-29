#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/ItemData.h"
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
	
	int m_nCrntLevel;
	
	FName m_CrntTarget;

	FName m_CrntMat;

protected:
	void EnchantSuccess(bool isSpecial);

	void EnchantFail();

	bool TryEnchant();

public:
	int GetEnchantCost() const;

	int GetCrntLevel() const;

	float GetEnchantPercent() const;
	
	void SetTargetEquip(const FName& target, UInventory* inven);

	void SetMaterialEquip(const FName& mat, UInventory* inven);

	const FName& GetCrntTarget() const;

	const FName& GetCrntMat() const;

	bool IsAbleTarget(const FName& target);

	bool IsAbleMaterial(const FName& material);

	void Clear();
	
	void DoEnchant();

	bool IsEnchantAvailable() const;
};
