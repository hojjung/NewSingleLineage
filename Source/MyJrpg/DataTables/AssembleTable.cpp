// Fill out your copyright notice in the Description page of Project Settings.


#include "AssembleTable.h"

UDataTable* UAssembleTable::GetAssembleStage = nullptr;

UAssembleTable::UAssembleTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Item/CurrencyTable.CurrencyTable'"));

	GetAssembleStage = FoundTable01.Object;
}
