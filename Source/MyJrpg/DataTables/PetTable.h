// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCPaletteDataAsset.h"
#include "UnitEntityData.h"
#include "UObject/NoExportTypes.h"
#include "PetTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UPetTable : public UObject
{
	GENERATED_BODY()

public:
	UPetTable();

	static UDataTable* GetPetTable;
};

USTRUCT(BlueprintType)
struct FPetRow : public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UUnitEntityAsset> m_PetAsset;
};