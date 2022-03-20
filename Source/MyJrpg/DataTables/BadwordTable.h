#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "BadwordTable.generated.h"

struct FBadWordDataRow;
/**
 * 
 */
UCLASS()
class MYJRPG_API UBadwordTable : public UObject
{
	GENERATED_BODY()
public:
	UBadwordTable();

	static UDataTable* GetBadwordTable;

	TArray<FBadWordDataRow*> m_AryBadwordList;

public:
	void Init();
	
	void ChatFilterBadWord(FString& outChatWant);

	bool NicknameHasBadWord(const FString& nameWant);
};


USTRUCT(BlueprintType)
struct FBadWordDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString m_Badword;
};