#include "StructureActor.h"
#include "MyJrpg/DataTables/BuildData.h"

AStructureActor::AStructureActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetMobility(EComponentMobility::Movable);
	m_WidgetComp = CreateDefaultSubobject<UBuildWidgetCompo>(TEXT("m_WidgetComp"));
	m_WidgetComp->SetupAttachment(RootComponent);
	m_WidgetComp->SetRelativeLocation(FVector(0,0,300));
	m_DataRow = nullptr;
}

void AStructureActor::BeginPlay()
{
	Super::BeginPlay();

	m_AryMats = GetStaticMeshComponent()->GetMaterials();
}

bool AStructureActor::HasNextUpgrade()
{
	return !m_DataRow->m_NextUpgradeActorID.IsNone();
}

void AStructureActor::SetBuildData(const FBuildDataRow& data_row)
{
	m_DataRow = &data_row;
	m_WidgetComp->Init();
}

void AStructureActor::SetMat(UMaterialInterface* mat)
{
	int Iter = 0;

	if (mat)
	{
		while (Iter < GetStaticMeshComponent()->GetMaterials().Num())
		{
			GetStaticMeshComponent()->SetMaterial(Iter, mat);
			Iter++;
		}
	}
	else
	{
		while (Iter < GetStaticMeshComponent()->GetMaterials().Num())
		{
			GetStaticMeshComponent()->SetMaterial(Iter,m_AryMats[Iter]);
			Iter++;
		}	
	}
}

void AStructureActor::ConfirmBuild()
{
	SetMat(nullptr);

	SetActorEnableCollision(true);

	m_WidgetComp->SetVisibility(false);
}

const FBuildDataRow& AStructureActor::GetBuildData() const
{
	return *m_DataRow;
}

void AStructureActor::ShowBuildWidget(bool b)
{
	m_WidgetComp->ShowBuildWidget(b);
}

void AStructureActor::ShowSelect(bool b)
{
	m_WidgetComp->ShowSelect(b);
}

bool AStructureActor::TryUpgrade()
{
	return true;
}

void AStructureActor::SetColl(bool b)
{
	if(b)
	{
		GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
