#include "ZoneData.h"
#include "UObject/ConstructorHelpers.h"

#include "NPCPaletteDataAsset.h"
#include "MyJrpg/GameRules/Rule_Default.h"


UDataTable* UZoneData::GetZoneTable = nullptr;

UZoneData::UZoneData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable(
		TEXT("DataTable'/Game/00_DataTables/ZoneTable.ZoneTable'"));
	//
	GetZoneTable = FoundTable.Object;
}

FZoneDataRow::FZoneDataRow()
{
	m_ClassGameRule = URule_Default::StaticClass();
}

