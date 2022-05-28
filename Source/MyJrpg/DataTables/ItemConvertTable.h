// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ItemConvertTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UItemConvertTable : public UObject
{
	GENERATED_BODY()

public:
	UItemConvertTable();
	
	static UDataTable* GetItemConverter;
};

USTRUCT(BlueprintType)
struct FItemConvertSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemDataHandle m_LeftItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nLeftItemStLv = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemDataHandle m_RightItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nRightItemStLv = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fConvertingTime = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemDataHandle m_CostItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nCostItemStLv = 0;
};

USTRUCT(BlueprintType)
struct FFuelData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemDataHandle m_FuelItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fFuelDuration = 0;
};

USTRUCT(BlueprintType)
struct FItemConvertRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextConverterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFuelData> m_AryFuelItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemConvertSet> m_AryItems;
};