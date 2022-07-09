// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EventStageTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UEventStageTable : public UObject
{
	GENERATED_BODY()
public:
	UEventStageTable();
	
	static UDataTable* GetEventStage;
};

USTRUCT(BlueprintType)
struct FEventStageRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D m_Coord;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextEventName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextEventDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_EventImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_ZoneID;
};