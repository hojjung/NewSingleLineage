#include "StructureActor.h"
#include "MyJrpg/DataTables/BuildData.h"

AStructureActor::AStructureActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetMobility(EComponentMobility::Movable);
}

void AStructureActor::BeginPlay()
{
	Super::BeginPlay();

	m_AryMats = GetStaticMeshComponent()->GetMaterials();
}

void AStructureActor::SetBuildData(const FBuildDataRow& data_row)
{
	
}

void AStructureActor::SetMat(UMaterialInterface* mat)
{
	int Iter = 0;

	while (Iter < GetStaticMeshComponent()->GetMaterials().Num())
	{
		GetStaticMeshComponent()->SetMaterial(Iter,mat);
		Iter++;
	}
}

void AStructureActor::ConfirmBuild()
{
	int Iter = 0;

	while (Iter < GetStaticMeshComponent()->GetMaterials().Num())
	{
		GetStaticMeshComponent()->SetMaterial(Iter,m_AryMats[Iter]);
		Iter++;
	}

	SetActorEnableCollision(true);
}

