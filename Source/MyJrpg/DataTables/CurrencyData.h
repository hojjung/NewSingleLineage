// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyJrpg/Quest/Reward/RewardLogicBase.h"
#include "UObject/NoExportTypes.h"
#include "CurrencyData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UCurrencyData : public UObject
{
	GENERATED_BODY()

public:
	UCurrencyData();

	static UDataTable* GetCurrencyTable;
	
};
USTRUCT(BlueprintType)
struct FCurrencyDataRow : public FTableRowBase
{
	GENERATED_BODY()
	//퀘스트보상으로 줄수있는 모든것,이미지 필요,장비템도 일일히 만든다고?,재화인지 아이템인지 구분필요
	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;//경험치와 골드가 얻는 함수가 다르다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URewardLogicBase> m_ClassReceiveLogic;
};

USTRUCT(BlueprintType)
struct FCurrencyDataHandle : public FDataTableRowHandle
{
	GENERATED_BODY()

public:
	FCurrencyDataHandle();
};