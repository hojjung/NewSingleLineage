// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemExecuteBase.h"
#include "Exe_EnchantArmor.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UExe_EnchantArmor : public UItemExecuteBase
{
	GENERATED_BODY()
	
protected:
	virtual void Use(FItemSpec& item) const override;
};
