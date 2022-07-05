#include "SpawnEventTable.h"

UDataTable* USpawnEventTable::GetSpawnEventTable = nullptr;

USpawnEventTable::USpawnEventTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Unit/SpawnDataTable.SpawnDataTable'"));
	//
	GetSpawnEventTable = Found.Object;
}
