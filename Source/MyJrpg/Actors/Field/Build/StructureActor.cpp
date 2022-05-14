#include "StructureActor.h"
#include "MyJrpg/DataTables/BuildData.h"

AStructureActor::AStructureActor()
{
	m_BuildData = nullptr;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	m_WidgetComp = CreateDefaultSubobject<UBuildWidgetCompo>(TEXT("m_WidgetComp"));
	m_WidgetComp->SetupAttachment(RootComponent);
	m_WidgetComp->SetRelativeLocation(FVector(0,0,300));

	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshComp"));//22
	m_MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_MeshComp->CanCharacterStepUpOn = ECB_No;
	m_MeshComp->SetCanEverAffectNavigation(false);
	m_MeshComp->SetupAttachment(RootComponent);
	m_MeshComp->bReceivesDecals = false;
	m_MeshComp->SetMobility(EComponentMobility::Movable);
}

void AStructureActor::SetBuildData(const FBuildDataRow& data)
{
	m_BuildData = &data;

	if(!m_BuildData->m_Mesh.ToSoftObjectPath().IsNull())
	{
		m_MeshComp->SetStaticMesh(m_BuildData->m_Mesh.LoadSynchronous());
		m_MeshComp->SetRelativeLocation(m_BuildData->m_Offset);
		m_MeshComp->SetRelativeScale3D(FVector(m_BuildData->m_fScale));
		m_MeshComp->SetRelativeRotation(FRotator(0,m_BuildData->m_fRotYaw,0));
	}

	if(data.m_ClassInter->IsValidLowLevel())
	{
		m_BuildInteract = NewObject<UBuildInteractBase>(this, data.m_ClassInter);
		
		m_BuildInteract->Init();
	}

	GetComponents(m_AryMeshCompos);

	m_AryAryMats.Reset(5);
	
	for(UMeshComponent* MeshComp : m_AryMeshCompos)
	{
		m_AryAryMats.Emplace(MeshComp->GetMaterials());
	}

	m_WidgetComp->Init();
}

void AStructureActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_MeshComp->SetStaticMesh(nullptr);
}

const FBuildDataRow& AStructureActor::GetBuildData() const
{
	return *m_BuildData;
}



void AStructureActor::SetMat(UMaterialInterface* mat)
{
	int Iter = 0;

	int IterY = 0;

	if (mat)
	{
		while (Iter < m_AryMeshCompos.Num())
		{
			while (IterY < m_AryMeshCompos[Iter]->GetMaterials().Num())
			{
				m_AryMeshCompos[Iter]->SetMaterial(IterY, mat);
				IterY++;
			}
			IterY = 0;
			Iter++;
		}
	}
	else
	{
		while (Iter < m_AryMeshCompos.Num())
		{
			while (IterY < m_AryMeshCompos[Iter]->GetMaterials().Num())
			{
				m_AryMeshCompos[Iter]->SetMaterial(IterY, m_AryAryMats[Iter][IterY].Get());
				IterY++;
			}
			IterY = 0;
			Iter++;
		}
	}
}

void AStructureActor::ShowBuildWidget(bool b)
{
	m_WidgetComp->ShowBuildWidget(b);
}

void AStructureActor::ConfirmBuild()
{
	m_WidgetComp->SetVisibility(false);

	SetMat(nullptr);

	SetActorEnableCollision(true);
}

void AStructureActor::ShowSelect(bool b)
{
	m_WidgetComp->ShowSelect(b);
}

void AStructureActor::SetColl(bool b)
{
	if(b)
	{
		m_MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AStructureActor::IsUpgradeable()
{
	return !GetBuildData().m_NextUpgradeActorID.IsNone();
}

void AStructureActor::OnInteract()
{
	m_BuildInteract->OnInteract();
}

float AStructureActor::GetBoundHalfHeight()
{
	FVector Min,Max;
	m_MeshComp->GetLocalBounds(Min,Max);

	return Max.Z - Min.Z;
}

bool AStructureActor::IsInteractImplemented()
{
	return m_BuildInteract != nullptr;
}

bool AStructureActor::IsEraseable()
{
	return m_BuildInteract ? m_BuildInteract->IsEraseable() : true;
}
