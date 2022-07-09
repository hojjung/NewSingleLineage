// Fill out your copyright notice in the Description page of Project Settings.


#include "EventStageTable.h"
UDataTable* UEventStageTable::GetEventStage = nullptr;
//

UEventStageTable::UEventStageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Item/CurrencyTable.CurrencyTable'"));

	GetEventStage = FoundTable01.Object;
}
