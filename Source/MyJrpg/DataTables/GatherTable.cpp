#include "GatherTable.h"

UDataTable* UGatherTable::GetGatherTable = nullptr;

UGatherTable::UGatherTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Gather/GatherTable.GatherTable'"));
	//
	GetGatherTable = Found.Object;
}
