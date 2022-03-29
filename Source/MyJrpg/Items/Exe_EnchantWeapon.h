// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemExecuteBase.h"
#include "Exe_EnchantWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UExe_EnchantWeapon : public UItemExecuteBase
{
	GENERATED_BODY()

protected:
	virtual void Use(const FName& item,UInventory* inven) const override;
};


