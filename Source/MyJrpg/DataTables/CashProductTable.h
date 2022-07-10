// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildData.h"
#include "CurrencyData.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "CashProductTable.generated.h"

UENUM(BlueprintType)
enum class ECashCategory : uint8
{
	Package,
	Build,
	Equipment,
	Misc,
	Currency,
	Length UMETA(Hidden)
};

UCLASS()
class MYJRPG_API UCashProductTable : public UObject
{
	GENERATED_BODY()
public:
	UCashProductTable();
	
	static UDataTable* GetCashTable;
};

USTRUCT(BlueprintType)
struct FCashProductDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	ECashCategory m_Category;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_ProductName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_ProductDesc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_KRW;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fSale = 0.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FItemDataHandle> m_ItemDatas;//우편함 아이템 최대개수
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FBuildDataHandle> m_BuildDatas;//최대 보유 개수?
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FCurrencyDataHandle> m_CashDatas;
};
