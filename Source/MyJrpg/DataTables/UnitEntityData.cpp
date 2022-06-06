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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundHitEffect(
		TEXT("ParticleSystem'/Game/03_VisualEffect/P_Hit.P_Hit'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> FoundHitSound(
		TEXT("SoundWave'/Game/Sound/Fantasy_Game_Weapon_Impact.Fantasy_Game_Weapon_Impact'"));

	m_TakeHitSound = FoundHitSound.Object;

	m_TakeHitEffect = FoundHitEffect.Object;
}

	