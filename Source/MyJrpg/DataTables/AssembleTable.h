// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "AssembleTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UAssembleTable : public UObject
{
	GENERATED_BODY()
public:
	UAssembleTable();
	
	static UDataTable* GetAssembleStage;
};
//필요 없지 않나>
USTRUCT(BlueprintType)
struct FAssembleCraft : public FCraftable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_UpgradeIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D m_Coord;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextEventName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextEventDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_ZoneID;
};