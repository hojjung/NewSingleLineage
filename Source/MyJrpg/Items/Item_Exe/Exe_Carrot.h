// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemExecuteBase.h"
#include "Exe_Carrot.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UExe_Carrot : public UItemExecuteBase
{
	GENERATED_BODY()

protected:
	void TryGetSeedItem() const;

public:
	virtual void Use(FItemSpec& item,UInventory* inven) const override;
};
