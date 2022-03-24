// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/MyJrpg.h"
#include "UObject/NoExportTypes.h"
#include "OptionBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UOptionBase : public UObject
{
	GENERATED_BODY()

protected:
	int m_AryEnchantValue [FGlobalVariable::ENCHANT_MAX];

public:
	virtual bool IsPercent()
	{
		return false;//if percent, int 4 is +4%
	}

	int GetEnchantValue(int enchantLevel)
	{
		return m_AryEnchantValue[enchantLevel];
	}

	virtual FString GetFormatStr()
	{
		return FString();
	}
};
