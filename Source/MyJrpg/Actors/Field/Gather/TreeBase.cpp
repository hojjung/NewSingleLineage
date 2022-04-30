// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeBase.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

ATreeBase::ATreeBase()
{
	m_Capsule->InitCapsuleSize(100, 130);
	m_Capsule->SetRelativeRotation(FRotator(0,180,0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	FoundTree(TEXT("StaticMesh'/Game/08_EnvironmentMesh/MyGather/Tree09Top.Tree09Top'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	FoundTrunk(TEXT("StaticMesh'/Game/08_EnvironmentMesh/MyGather/Tree09Btm.Tree09Btm'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	FoundAnim(TEXT("AnimMontage'/Game/09_SharedAnimations/Player/Gather_Axe.Gather_Axe'"));

	static ConstructorHelpers::FObjectFinder<USoundBase>
	FoundSound(TEXT("SoundWave'/Game/Sound/Tree_Falling_4.Tree_Falling_4'"));
	
	m_MeshTree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTree"));
	m_MeshTree->CanCharacterStepUpOn = ECB_No;
	m_MeshTree->SetCanEverAffectNavigation(false);
	m_MeshTree->SetupAttachment(m_Capsule);
	m_MeshTree->bReceivesDecals = false;

	m_MeshTree->SetStaticMesh(FoundTree.Object);
	m_MeshTree->SetMobility(EComponentMobility::Movable);
	m_MeshTree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshTree->SetCollisionProfileName(TEXT("PhysicsActor"));

	m_MeshTrunk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTrunk"));
	m_MeshTrunk->CanCharacterStepUpOn = ECB_No;
	m_MeshTrunk->SetCanEverAffectNavigation(false);
	m_MeshTrunk->SetupAttachment(m_Capsule);
	m_MeshTrunk->bReceivesDecals = false;

	m_MeshTrunk->SetStaticMesh(FoundTrunk.Object);
	m_MeshTrunk->SetMobility(EComponentMobility::Movable);
	m_MeshTrunk->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Motion = FoundAnim.Object;

	m_nTreeHp = 3;

	m_Sound = FoundSound.Object;

	PrimaryActorTick.bCanEverTick = true;

	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
			TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(RootComponent);
	m_ShadowMeshComp->SetRelativeLocation(FVector(0,0,5.f));
	m_ShadowMeshComp->SetRelativeScale3D(FVector(10));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
}

void ATreeBase::BeginPlay()
{
	Super::BeginPlay();
	m_Player = UMyLib::GetPlayer();
	UMyGameInstance::Get->m_SpawnManager->AddFocusActor(this);
}

void ATreeBase::OnInteract()
{
	if(m_Player->GetInteracting())
	{
		return;
	}

	m_Player->PlayAnimMontage(m_Motion);
	m_Player->HomingRotateToTarget(0);
	m_Player->SetInteracting(true);
}

void ATreeBase::OnHarvestMotionDone()
{
	m_Player->SetInteracting(false);
}

void ATreeBase::OnTakeChopping()
{
	PRINTF("TookChopping!");

	m_nTreeHp--;
	//Effect
	if(m_nTreeHp<=0)
	{
		OnGatherDone();
		UMyGameInstance::Get->m_SpawnManager->RemoveFocusActor(this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),m_Sound,GetActorLocation());
		StartDeathEffectMaterial(8);
		//Dissolve
	}
}

void ATreeBase::OnGatherDone()
{
	m_MeshTree->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	m_MeshTree->SetSimulatePhysics(true);
}

void ATreeBase::CreateSetDeathCurve(float fullLength)
{
	m_CurveDeathAnim =FFloatCurve(); 
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
