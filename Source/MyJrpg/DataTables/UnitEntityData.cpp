#include "UnitEntityData.h"

#include "MyJrpg/Pawns/MonsterPawn.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* UUnitEntityData::GetNpcUnitTable = nullptr;

UUnitEntityData::UUnitEntityData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable03(TEXT("DataTable'/Game/00_DataTables/Unit/NpcUnitTable.NpcUnitTable'"));
	//
	GetNpcUnitTable = FoundTable03.Object;
}

USkeletalMesh* UUnitEntityAsset::GetSkMesh() const
{
	return m_BodyMesh;
}

UAnimMontage* UUnitEntityAsset::GetBaseAtkAnim() const
{
	return m_BaseAttackAnim;
}

FNpcUnitEntityRow::FNpcUnitEntityRow()
{
	m_ClassActor = AMonsterPawn::StaticClass();


}

	