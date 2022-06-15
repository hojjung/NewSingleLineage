// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewActor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"


APreviewActor::APreviewActor()
{
	PrimaryActorTick.bCanEverTick = true;
	//init skMesh
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	//
	m_MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	m_MeshBody->SetupAttachment(RootComponent);
	m_MeshBody->SetCollisionProfileName(TEXT("NoCollision"));
	m_MeshBody->SetGenerateOverlapEvents(false);
	m_MeshBody->SetCanEverAffectNavigation(false);
	m_MeshBody->SetRelativeLocation(FVector(0, 0, -88));
	m_MeshBody->SetRelativeRotation(FRotator(0, -90.f, 0.f));
	m_MeshBody->SetCollisionProfileName(TEXT("CharacterMesh"));
	m_MeshBody->bReceivesDecals = false;
	m_MeshBody->bOwnerNoSee = false;
	//
	m_MeshBody->bCastDynamicShadow = false; //chanage for mobile
	m_MeshBody->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;//최적화
	m_MeshBody->bAffectDynamicIndirectLighting = true;
	m_MeshBody->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshBody->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_MeshBody->bEnableUpdateRateOptimizations = true;
	m_MeshBody->bComponentUseFixedSkelBounds = true;
	//
	m_Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	m_Spring->SetupAttachment(RootComponent);
	m_Spring->SetRelativeRotation(FRotator(-5, 200.f, 0));
	m_Spring->TargetArmLength = 200;
	m_Spring->bDoCollisionTest = 0;

	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	m_Capture->SetupAttachment(m_Spring);
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FoundTexture(TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_PlayerVisual.T_PlayerVisual'"));
	m_CaptureTexture = FoundTexture.Object;
	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	m_Capture->SetTickableWhenPaused(true);
	m_Capture->ProjectionType = ECameraProjectionMode::Perspective;
	m_Capture->FOVAngle = 60.f;
	//
	m_Light = CreateDefaultSubobject<UPointLightComponent>("m_Light");
	m_Light->SetupAttachment(RootComponent);
	m_Light->SetRelativeLocation(FVector(130,80,80));
	m_Light->CastShadows = false;
	//
	m_bTouched = false;
	//
	m_MeshLeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshLeftHand"));
	m_MeshLeftHand->SetupAttachment(m_MeshBody);
	m_MeshLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshLeftHand->bCastDynamicShadow = false;
	m_MeshLeftHand->bAffectDynamicIndirectLighting = true;
	m_MeshLeftHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshLeftHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	//
	m_MeshRightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshRightHand"));
	m_MeshRightHand->SetupAttachment(m_MeshBody);
	m_MeshRightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshRightHand->bCastDynamicShadow = false;
	m_MeshRightHand->bAffectDynamicIndirectLighting = true;
	m_MeshRightHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshRightHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}

void APreviewActor::BeginPlay()
{
	Super::BeginPlay();

	m_Capture->TextureTarget = m_CaptureTexture;
	
	m_Capture->PostProcessBlendWeight = 0.f;

	m_Capture->ShowOnlyActors.Add(this);

	m_InitVisualRot = m_MeshBody->GetComponentRotation();

	HideMeshWithTick();
}

void APreviewActor::SetEntity(TSoftObjectPtr<UUnitEntityAsset> asset)
{
	const UUnitEntityAsset* entityData = UMyAssetManager::Get()->LoadUnitAsset(asset);
	
	m_MeshBody->SetSkeletalMesh(entityData->m_BodyMesh);
	m_MeshBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_MeshBody->SetAnimClass(entityData->m_AnimBP);
	m_MeshBody->AddRelativeRotation(FRotator(0,entityData->m_RotYawOffset,0));
}

void APreviewActor::SetMeshScale(float s)
{
	m_MeshBody->SetRelativeScale3D(FVector(s));
}

void APreviewActor::RemoveRightActor()
{
	if(!m_ActorRightHand.Get())
	{
		return;
	}
	m_Capture->ShowOnlyActors.Remove(m_ActorRightHand.Get());
	m_ActorRightHand->Destroy();
}

void APreviewActor::RemoveLeftActor()
{
	if(!m_ActorLeftHand.Get())
	{
		return;
	}
	m_Capture->ShowOnlyActors.Remove(m_ActorLeftHand.Get());
	m_ActorLeftHand->Destroy();
}

void APreviewActor::OnMeshVisualChanged(const AModularUnitPawn* charData)
{
	m_MeshBody->SetSkeletalMesh(charData->GetSkMesh()->SkeletalMesh,false);

	UStaticMesh* LeftMesh = charData->GetLeftWeaponMesh()->GetStaticMesh();

	AAttachedWeapon* LeftActor = charData->GetLeftWeaponActor();

	if(LeftMesh)
	{
		m_MeshLeftHand->SetStaticMesh(LeftMesh);
		RemoveLeftActor();
	}
	else if (LeftActor)
	{
		m_MeshLeftHand->SetStaticMesh(nullptr);
		if(m_ActorLeftHand.Get() && LeftActor->GetClass() == m_ActorLeftHand->GetClass())
		{
			return;
		}
		RemoveLeftActor();
		m_ActorLeftHand = GetWorld()->SpawnActor<AAttachedWeapon>(LeftActor->GetClass());
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false);
		m_ActorLeftHand->AttachToComponent(m_MeshLeftHand,Rule);
		m_Capture->ShowOnlyActors.Add(m_ActorLeftHand.Get());
	}
	else
	{
		m_MeshLeftHand->SetStaticMesh(nullptr);
		RemoveLeftActor();
	}

	UStaticMesh* RightMesh = charData->GetRightWeaponMesh()->GetStaticMesh();

	AAttachedWeapon* RightActor = charData->GetRightWeaponActor();

	if(RightMesh)
	{
		m_MeshRightHand->SetStaticMesh(RightMesh);
		RemoveRightActor();
	}
	else if(RightActor)
	{
		m_MeshRightHand->SetStaticMesh(nullptr);
		if(m_ActorRightHand.Get() && RightActor->GetClass() == m_ActorRightHand->GetClass())
		{
			return;
		}
		RemoveRightActor();
		m_ActorRightHand = GetWorld()->SpawnActor<AAttachedWeapon>(RightActor->GetClass());
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false);
		m_ActorRightHand->AttachToComponent(m_MeshRightHand,Rule);
		m_Capture->ShowOnlyActors.Add(m_ActorRightHand.Get());
	}
	else
	{
		m_MeshRightHand->SetStaticMesh(nullptr);
		RemoveRightActor();
	}
}

void APreviewActor::OnMeshVisualChanged(const FUnitEntityRow& charData)
{
	SetEntity(charData.m_UnitDataAsset);
}

void APreviewActor::ShowMeshWithTick()
{
	m_MeshBody->SetVisibility(true);

	m_MeshBody->SetComponentTickEnabled(true);

	m_Capture->SetComponentTickEnabled(true);

	PRINTF("ShowMeshWithTick");
}

void APreviewActor::HideMeshWithTick()
{
	m_MeshBody->SetVisibility(false);

	m_MeshBody->SetComponentTickEnabled(false);

	m_Capture->SetComponentTickEnabled(false);

	PRINTF("HideMeshWithTick");
}

void APreviewActor::SetIsTouched(bool b)
{
	m_bTouched = b;
}

void APreviewActor::RotatePawn(float delta_x)
{
	delta_x = -1.f * delta_x;
	FRotator Rot(0.f);
	Rot.Yaw = delta_x;
	m_MeshBody->AddLocalRotation(Rot);
}

void APreviewActor::SetupPlayerPreview()
{
	UHumanAsset* Asset = UMyGameInstance::Get->m_PlayerStatManager->GetUnitAsset().Get();
	
	m_MeshBody->SetSkeletalMesh(Asset->m_BodyMesh);
	m_MeshBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_MeshBody->SetAnimInstanceClass(Asset->m_AnimBP);
	
	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
	
	m_MeshLeftHand->AttachToComponent(m_MeshBody, Rules, TEXT("LeftHandSocket"));
	m_MeshRightHand->AttachToComponent(m_MeshBody, Rules, TEXT("RightHandSocket"));
}

void APreviewActor::Tick(float delta)
{
	Super::Tick(delta);

	CalculateVisualActorRot(delta);
}

void APreviewActor::CalculateVisualActorRot(float delta)
{
	if (m_bTouched)
	{
		return;
	}

	FRotator NewRot = m_MeshBody->GetComponentRotation();

	NewRot.Yaw = UKismetMathLibrary::RInterpTo(NewRot, m_InitVisualRot, delta, 5.5f).Yaw;

	m_MeshBody->SetWorldRotation(NewRot);
}

