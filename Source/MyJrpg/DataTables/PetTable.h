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
struct FPetRow : public FUnitEntityRow
{
	GENERATED_BODY()

public://스텟과 보상
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColorDataHandle m_Color;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UOptionBase>> m_Options;
};