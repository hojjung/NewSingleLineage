// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Interfaces/Buildable.h"
#include "UObject/NoExportTypes.h"
#include "BuildData.generated.h"

class AStructureActor;
/**
 * 
 */
UCLASS()
class MYJRPG_API UBuildData : public UObject
{
	GENERATED_BODY()

public:
	UBuildData();
	
	static UDataTable* GetBuildTable;
};

USTRUCT()
struct FBuildDataRow : public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EBuildType m_BuildType;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCraftLevelLimit=1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FCraftItemCost> m_AryCostItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MustImplement = "Buildable") )
	TSubclassOf<AActor> m_ClassActor;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_NextUpgradeActorID;
};