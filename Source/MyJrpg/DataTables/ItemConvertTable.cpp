#include "ItemConvertTable.h"

UDataTable* UItemConvertTable::GetItemConverter = nullptr;

UItemConvertTable::UItemConvertTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable(TEXT("DataTable'/Game/00_DataTables/Item/ItemConvertTable.ItemConvertTable'"));
	//
	GetItemConverter = FoundTable.Object;
}
