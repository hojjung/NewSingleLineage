// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff_Base.h"
#include "Buff_Test.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBuff_Test : public UBuff_Base
{
	GENERATED_BODY()

protected:
	virtual void OnTick(float delta) override;

	virtual void Init(const FBuffDataRow& data) override;

	virtual void EndBuff() override;
};
