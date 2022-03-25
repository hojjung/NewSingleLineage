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
	int m_nLevelPerValue;

	bool m_bIsPercent;

	FString m_OpFormat;

	FString m_OpName;

public:
	virtual bool IsPercent()
	{
		return m_bIsPercent;//if percent, int 4 is +4%
	}

	int GetEnchantValue(int enchantLevel)
	{
		return m_nLevelPerValue * enchantLevel;
	}

	FString GetOptionFormat()
	{
		return m_OpFormat;
	}

	FString GetOptionName()
	{
		return m_OpName;
	}
};
