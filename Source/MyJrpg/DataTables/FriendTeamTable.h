// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "FriendTeamTable.generated.h"

/**
 * 카르마는, 우호,중립,적대 가 존재
 * 시작 카르마 따로 존재
 * 오르는법 내리는법?
 */
UCLASS()//KARMA_MAX
class MYJRPG_API UFriendTeamTable : public UObject
{
	GENERATED_BODY()
	
public:
	UFriendTeamTable();

	static UDataTable* GetFriendTeamTable;
};

USTRUCT(BlueprintType)
struct FFriendDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FriendTeamName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nInitKarma = 0;
};
