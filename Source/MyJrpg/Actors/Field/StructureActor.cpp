#include "StructureActor.h"

#include "MyJrpg/DataTables/BuildData.h"

AStructureActor::AStructureActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_Mesh"));
	m_Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStructureActor::BeginPlay()
{
	Super::BeginPlay();

	m_AryMats = m_Mesh->GetMaterials();
}

// Called every frame
void AStructureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStructureActor::SetBuildData(const FBuildDataRow& data_row)
{
	
}

void AStructureActor::SetMat(UMaterialInterface* mat)
{
	int Iter = 0;

	while (Iter < m_Mesh->GetMaterials().Num())
	{
		m_Mesh->SetMaterial(Iter,mat);
	}
}

void AStructureActor::ConfirmBuild()
{
	int Iter = 0;

	while (Iter < m_Mesh->GetMaterials().Num())
	{
		m_Mesh->SetMaterial(Iter,m_AryMats[Iter]);
	}
}

