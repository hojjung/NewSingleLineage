#include "PetTable.h"

UDataTable* UPetTable::GetPetTable = nullptr;

UPetTable::UPetTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Unit/PetUnitTable.PetUnitTable'"));
	//
	GetPetTable = Found.Object;
}
