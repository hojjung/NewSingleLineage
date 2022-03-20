// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcActor.h"

ANpcActor::ANpcActor()
{
	m_Capsule->InitCapsuleSize(34.0f, 300);
	
	m_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("m_MeshComp"));
	m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComp->CanCharacterStepUpOn = ECB_No;
	m_MeshComp->SetCanEverAffectNavigation(false);
	m_MeshComp->SetupAttachment(RootComponent);
	m_MeshComp->bReceivesDecals = false;

	m_MeshComp->SetRelativeLocation(FVector(0, 0, -88));
	m_MeshComp->SetRelativeRotation(FRotator(0, -90.f, 0.f));
	//
	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
			TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(m_MeshComp);
	m_ShadowMeshComp->SetRelativeLocation(FVector(0,0,5.f));
	m_ShadowMeshComp->SetRelativeScale3D(FVector(3.5f));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
}

void ANpcActor::BeginPlay()
{
	Super::BeginPlay();
	if(m_IdleAnim)
	{
		m_MeshComp->PlayAnimation(m_IdleAnim,true);
	}
}
