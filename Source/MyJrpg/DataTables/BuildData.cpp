#include "BuildData.h"

UDataTable* UBuildData::GetBuildTable = nullptr;

UBuildData::UBuildData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Build/BuildDataTable.BuildDataTable'"));
	//
	GetBuildTable = Found.Object;
}
