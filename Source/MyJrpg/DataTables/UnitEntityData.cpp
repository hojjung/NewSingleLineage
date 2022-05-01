#include "UnitEntityData.h"

#include "MyJrpg/Pawns/MonsterPawn.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* UUnitEntityData::GetPlayerUnitTable = nullptr;
UDataTable* UUnitEntityData::GetNpcUnitTable = nullptr;

UUnitEntityData::UUnitEntityData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Unit/PlayerUnitTable.PlayerUnitTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable03(TEXT("DataTable'/Game/00_DataTables/Unit/NpcUnitTable.NpcUnitTable'"));
	//
	GetPlayerUnitTable = FoundTable01.Object;
	GetNpcUnitTable = FoundTable03.Object;
}

FNpcUnitEntityRow::FNpcUnitEntityRow()
{
	m_ClassActor = AMonsterPawn::StaticClass();
}

