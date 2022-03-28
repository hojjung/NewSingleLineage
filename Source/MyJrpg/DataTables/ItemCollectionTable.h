// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "MyJrpg/Items/Options/OptionBase.h"
#include "UObject/NoExportTypes.h"
#include "ItemCollectionTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UItemCollectionTable : public UObject
{
	GENERATED_BODY()

public:
	UItemCollectionTable();
	
	static UDataTable* GetItemCollecTable;
};

USTRUCT(BlueprintType)
struct FItemCollecNeed
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemDataHandle m_Item;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nEnchantLv = 0;
};

USTRUCT(BlueprintType)
struct FItemCollecRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UOptionBase> m_ClassOption;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemCollecNeed> m_AryItems;
};
