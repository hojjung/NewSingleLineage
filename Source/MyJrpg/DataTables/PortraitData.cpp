#include "PortraitData.h"


#include "UObject/ConstructorHelpers.h"

UDataTable* UPortraitData::GetPortraitTable = nullptr;

UPortraitData::UPortraitData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable03(TEXT("DataTable'/Game/00_DataTables/PortraitTable.PortraitTable'"));
	//
	GetPortraitTable = FoundTable03.Object;
}
