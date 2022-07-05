// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitEntityData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "SpawnEventTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USpawnEventTable : public UObject
{
	GENERATED_BODY()
public:
	USpawnEventTable();

	static UDataTable* GetSpawnEventTable;
};


USTRUCT(BlueprintType)
struct FUnitDataHandle : public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitDataHandle()
	{
		DataTable = UUnitEntityData::GetNpcUnitTable;
	}
};

USTRUCT(BlueprintType)
struct FUnitSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fSpawnDelay = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FUnitDataHandle> m_Units;
};

USTRUCT(BlueprintType)
struct FSpawnDataRow : public FTableRowBase
{
	GENERATED_BODY()

public://Row 한개
	UPROPERTY(EditAnywhere)
	TArray<FUnitSpawnData> m_UnitSpawnDatas;
};