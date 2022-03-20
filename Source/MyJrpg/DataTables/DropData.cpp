#include "DropData.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* UDropData::GetDropTable = nullptr;

UDropData::UDropData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Item/DropTable.DropTable'"));
	//
	GetDropTable = FoundTable01.Object;
}

FDropDataRowHandle::FDropDataRowHandle()
{
	DataTable = UDropData::GetDropTable;
}