// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "BuffTable.generated.h"

class UBuff_Base;
/**
 * 
 */
UCLASS()
class MYJRPG_API UBuffTable : public UObject
{
	GENERATED_BODY()
public:
	UBuffTable();
	
	static UDataTable* GetBuffTable;
};

USTRUCT(BlueprintType)
struct FBuffDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBuff_Base> m_ClassBuff;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fDuration;
};
