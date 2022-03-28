#include "ItemCollectionTable.h"

UDataTable* UItemCollectionTable::GetItemCollecTable = nullptr;

UItemCollectionTable::UItemCollectionTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable(TEXT("DataTable'/Game/00_DataTables/Item/ItemCollection.ItemCollection'"));
	//
	GetItemCollecTable = FoundTable.Object;
}
