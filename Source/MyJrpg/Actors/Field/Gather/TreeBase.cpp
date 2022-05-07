// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeBase.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

ATreeBase::ATreeBase()
{
	PrimaryActorTick.bCanEverTick = true; 
	
	m_Capsule->InitCapsuleSize(50, 50.0f);

	m_MeshTree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTree"));
	m_MeshTree->CanCharacterStepUpOn = ECB_No;
	m_MeshTree->SetCanEverAffectNavigation(false);
	m_MeshTree->SetupAttachment(m_Capsule);
	m_MeshTree->bReceivesDecals = false;

	m_MeshTree->SetMobility(EComponentMobility::Movable);
	m_MeshTree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshTree->SetCollisionProfileName(TEXT("PhysicsActor"));
	m_MeshTree->CastShadow = false;
	m_MeshTree->bSelfShadowOnly = false;
	

	m_MeshTrunk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTrunk"));
	m_MeshTrunk->CanCharacterStepUpOn = ECB_No;
	m_MeshTrunk->SetCanEverAffectNavigation(false);
	m_MeshTrunk->SetupAttachment(m_Capsule);
	m_MeshTrunk->bReceivesDecals = false;

	m_MeshTrunk->SetMobility(EComponentMobility::Movable);
	m_MeshTrunk->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshTrunk->CastShadow = false;
	m_MeshTrunk->bSelfShadowOnly = false;
	

	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(RootComponent);
	m_ShadowMeshComp->SetRelativeScale3D(FVector(10));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
	m_ShadowMeshComp->SetRelativeLocation(FVector(0,0,-88));

	m_nTreeHp = 3;
}
//(X=0.000005,Y=20.000000,Z=-10.000000) btm
//
void ATreeBase::SetEntity(const FGatherDataRow& data, AMyPlayerPawn* pl)
{
	m_Player = pl;

	float H = GetBoundHalfHeight() * -1.f;
	m_MeshTree->SetRelativeLocation(FVector(0,0,H));
	m_MeshTrunk->SetRelativeLocation(FVector(0,0,H));
	
	m_GatherAsset = UMyAssetManager::Get()->LoadGatherAsset(data.m_GatherAsset);

	m_MeshTree->SetStaticMesh(m_GatherAsset->m_TopMesh);

	m_MeshTrunk->SetStaticMesh(m_GatherAsset->m_BtmMesh);

	StartDeathEffectMaterial(data.m_fDisappearDelay);

	m_MeshTree->SetRelativeScale3D(FVector(data.m_fTopMeshScale));
	
	m_MeshTrunk->SetRelativeScale3D(FVector(data.m_fBtmMeshScale));

	m_bUsePhysics = data.m_bUsePhysics;
}

void ATreeBase::OnInteract()
{
	if(m_Player->GetInteracting())
	{
		return;
	}
	if(m_nTreeHp<=0)
	{
		return;
	}

	m_Player->PlayAnimMontage(m_GatherAsset->m_AnimGatherMotion);
	m_Player->HomingRotateToTarget(0);
	m_Player->SetInteracting(true);
}

void ATreeBase::OnHarvestMotionDone()
{
	m_Player->SetInteracting(false);
}

void ATreeBase::OnTakeChopping()
{
	m_nTreeHp--;
	
	if(m_nTreeHp<=0)
	{
		OnGatherDone();
	}
}

void ATreeBase::OnGatherDone()
{
	UMyGameInstance::Get->m_SpawnManager->RemoveFocusActor(this);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),m_GatherAsset->m_SoundGatherEnd,GetActorLocation());
	m_MeshTree->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	if(m_bUsePhysics)
		m_MeshTree->SetSimulatePhysics(true);
}

void ATreeBase::CreateSetDeathCurve(float fullLength)
{
	m_CurveDeathAnim = FFloatCurve(); 
	m_CurveDeathAnim.UpdateOrAddKey(1, 0);
	m_CurveDeathAnim.UpdateOrAddKey(0, fullLength);
}

void ATreeBase::StartDeathEffectMaterial(float duration)
{
	m_fDeathAnimDurationMax = duration;

	m_fDeathAnimDurationTimer = 0.f;

	CreateSetDeathCurve(duration);
}

void ATreeBase::SetDeathEffectMaterial(float deltaTime)
{
	m_fDeathAnimDurationTimer+=deltaTime;

	float Value = m_CurveDeathAnim.Evaluate(m_fDeathAnimDurationTimer);

	FName MaskParam = TEXT("Visibility");

	m_MeshTree->SetScalarParameterValueOnMaterials(MaskParam, Value);
}
void ATreeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(m_nTreeHp<=0)
	{
		SetDeathEffectMaterial(DeltaSeconds);

		if(m_fDeathAnimDurationTimer>=m_fDeathAnimDurationMax)
		{
			SetActorTickEnabled(false);
			
			m_MeshTree->DestroyComponent();
			m_MeshTree = nullptr;
		}
	}
}

