#include "BuildData.h"

UDataTable* UBuildData::GetBuildTable = nullptr;

UBuildData::UBuildData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Item/ItemTable.ItemTable'"));
	//
	GetBuildTable = Found.Object;
}
