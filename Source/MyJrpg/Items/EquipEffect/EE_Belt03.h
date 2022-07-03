// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EE_Base.h"
#include "EE_Belt03.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UEE_Belt03 : public UEE_Base
{
	GENERATED_BODY()

protected:
	virtual void Equip() override;

	virtual void UnEquip() override;
};
