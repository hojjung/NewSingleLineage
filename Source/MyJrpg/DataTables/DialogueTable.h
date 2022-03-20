// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DialogueTable.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UDialogueTable : public UObject
{
	GENERATED_BODY()
public:
	UDialogueTable();
	
	static UDataTable* GetDialogueTable;

	static UDialogue* GetDialogue(FName id);
};

USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

public://각 아이템 별로,자신이 몇번에 한번꼴로 드랍되는지 써주기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDialogue* m_Dialogue;
};
