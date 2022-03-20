// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueTable.h"

UDataTable* UDialogueTable::GetDialogueTable = nullptr;

UDialogueTable::UDialogueTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/DialogueTable.DialogueTable'"));
	//
	GetDialogueTable = Found.Object;
}

UDialogue* UDialogueTable::GetDialogue(FName id)
{
	return GetDialogueTable->FindRow<FDialogueData>(id,"")->m_Dialogue;
}
