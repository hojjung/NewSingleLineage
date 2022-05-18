// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemExecuteBase.h"
#include "Exe_Potion01.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UExe_Potion01 : public UItemExecuteBase
{
	GENERATED_BODY()

public:
	virtual void Use() const override;
};
