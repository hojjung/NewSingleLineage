#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DropData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UDropData : public UObject
{
	GENERATED_BODY()
public:
	UDropData();
	
	static UDataTable* GetDropTable;
};

USTRUCT(BlueprintType)
struct FDropRewardItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FItemDataHandle m_Item;
	UPROPERTY(EditDefaultsOnly)
	int m_nExpectDropCount=5;//5번에 한번,20% 드랍이란뜻임
};

USTRUCT(BlueprintType)
struct FDropDataRow : public FTableRowBase
{
	GENERATED_BODY()

public://각 아이템 별로,자신이 몇번에 한번꼴로 드랍되는지 써주기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDropRewardItem> m_AryDropItem;
};

//핸들 만들어서 엔티티 테이블이 가질수있게 해줘야함
//던전마다 정해줄것
USTRUCT(BlueprintType)
struct FDropDataRowHandle : public FDataTableRowHandle
{
	GENERATED_BODY()

public:
	FDropDataRowHandle();
};