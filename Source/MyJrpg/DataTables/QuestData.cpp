// All right Reserve 2021 HereticByte


#include "QuestData.h"

UDataTable* UQuestData::GetMainQuestTable = nullptr;

UDataTable* UQuestData::GetSubQuestTable = nullptr;

UQuestData::UQuestData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Quest/MainQuestTable.MainQuestTable'"));

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable02(TEXT("DataTable'/Game/00_DataTables/Quest/SubQuestTable.SubQuestTable'"));
	//
	GetMainQuestTable = FoundTable01.Object;

	GetSubQuestTable = FoundTable02.Object;
}
