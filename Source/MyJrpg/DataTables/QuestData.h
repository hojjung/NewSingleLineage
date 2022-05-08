// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "CurrencyData.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "MyJrpg/Quest/QuestLogicBase.h"
#include "UObject/NoExportTypes.h"
#include "QuestData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestData : public UObject
{
	GENERATED_BODY()
public:
	UQuestData();
	
	static UDataTable* GetMainQuestTable;
	
	static UDataTable* GetSubQuestTable;
};

USTRUCT(BlueprintType)
struct FQuestReward//it canbe equip,item,exp,gold,diamond
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FCurrencyDataHandle m_Currency;
	UPROPERTY(EditDefaultsOnly)
	FItemDataHandle m_Item;
	UPROPERTY(EditDefaultsOnly)
	int m_nAmount=1;
};

USTRUCT(BlueprintType)
struct FQuestDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_QuestName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true") )
	FText m_QuestDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_TargetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxTargetAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UQuestLogicBase> m_ClassQuestRegister;//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuestReward> m_AryQuestReward;
};

USTRUCT(BlueprintType)
struct FQuestSpec
{
	GENERATED_USTRUCT_BODY()
public:
	FQuestSpec()
	{
		Clear();
	}

	FQuestSpec(FName id,const FQuestDataRow& quest_data_row)
	{
		m_QuestID = id;

		m_nValue=0;

		m_QuestData = &quest_data_row;
	}

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName m_QuestID;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_nValue;

	const FQuestDataRow* m_QuestData;

	FORCEINLINE bool IsEmpty()
	{
		return !m_QuestData;
	}

	FORCEINLINE void Clear()
	{
		m_QuestID = NAME_None;

		m_nValue = -1;

		m_QuestData = nullptr;
	}
};
