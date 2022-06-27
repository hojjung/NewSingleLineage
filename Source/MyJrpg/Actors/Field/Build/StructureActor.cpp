#include "StructureActor.h"

#include "MyJrpg/MyLib.h"
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
	//
	m_IconMeshComp = CreateDefaultSubobject<UMinimapIconComp>("m_IconMeshComp");
	m_IconMeshComp->SetupAttachment(RootComponent);
}

void AStructureActor::SetBuildData(const FBuildDataRow& data)
{
	m_BuildData = &data;
	
	if(m_BuildData->m_BuildType == EBuildType::Foundation)
	{
		m_MeshComp->bReceivesDecals = true;
	}

	if (!m_BuildData->m_Mesh.ToSoftObjectPath().IsNull())
	{
		m_MeshComp->SetStaticMesh(m_BuildData->m_Mesh.LoadSynchronous());
	}
	m_MeshComp->SetRelativeLocation(m_BuildData->m_Offset);
	m_MeshComp->SetRelativeScale3D(FVector(m_BuildData->m_fScale));
	m_MeshComp->SetRelativeRotation(FRotator(0, m_BuildData->m_fRotYaw, 0));

	GetComponents(m_AryMeshCompos);

	m_AryAryMats.Reset(5);
	
	for(UMeshComponent* MeshComp : m_AryMeshCompos)
	{
		m_AryAryMats.Emplace(MeshComp->GetMaterials());

		if(MeshComp->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
		{
			m_AryMeshComposColl.Add(MeshComp);
		}
	}

	m_WidgetComp->Init();

	m_IconMeshComp->SetRotationOffset(FRotator(0,-45,0));
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

void AStructureActor::ConfirmBuild(UInventory* inven)
{
	m_WidgetComp->SetVisibility(false);

	SetMat(nullptr);

	SetActorEnableCollision(true);
	//
	if(m_BuildData->m_ClassInter->IsValidLowLevel())
	{
		m_BuildInteract = NewObject<UBuildInteractBase>(this, m_BuildData->m_ClassInter);
		m_BuildInteract->Init(m_BuildData->m_AryInteractVariable,inven);
	}
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

		for(UMeshComponent* Mc : m_AryMeshComposColl)
		{
			Mc->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	else
	{
		m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		for(UMeshComponent* Mc : m_AryMeshComposColl)
		{
			Mc->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

bool AStructureActor::IsUpgradeable()
{
	return !GetBuildData().m_NextUpgradeActorID.IsNone();
}

void AStructureActor::OnInteract()
{
	float R = 0;
	
	float H = 0;
	
	GetSimpleCollisionCylinder(R, H);
	
	UMyLib::GetPlayer()->RequestInteract(this,FVoidVoid::CreateUObject(this,&AStructureActor::OnArrived), R + 80);
}

void AStructureActor::OnArrived()
{
	m_BuildInteract->OnInteract();
}

bool AStructureActor::IsInteractImplemented()
{
	return m_BuildInteract != nullptr;
}

FVector AStructureActor::GetNavAgentLocation() const
{
	float R = 0;
	
	float H = 0;

	GetSimpleCollisionCylinder(R, H);

	return GetActorLocation() - FVector(0.f, 0.f, H);
}

void AStructureActor::GetMoveGoalReachTest(const AActor* MovingActor, const FVector& MoveOffset, FVector& GoalOffset,float& GoalRadius, float& GoalHalfHeight) const
{
	GoalOffset = m_MeshComp->GetRelativeLocation();
	
	GetSimpleCollisionCylinder(GoalRadius, GoalHalfHeight);
}

UInventory* AStructureActor::GetItemHolder()
{
	if(!m_BuildInteract)
	{
		return nullptr;
	}
	return m_BuildInteract->GetItemHolder();
}

FText AStructureActor::GetTextInteract()
{
	return NSLOCTEXT("AStructureActor","Control","조작");
}

UMinimapIconComp* AStructureActor::GetIconMeshComp()
{
	return m_IconMeshComp;
}

bool AStructureActor::IsEraseable()
{
	return m_BuildInteract ? m_BuildInteract->IsEraseable() : true;
}
