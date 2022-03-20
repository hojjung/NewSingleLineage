// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActorBase.h"

#include "Engine/CollisionProfile.h"
#include "MyJrpg/MyJrpg.h"

// Sets default values
AInteractActorBase::AInteractActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule00"));
	m_Capsule->InitCapsuleSize(34.0f, 88.0f);
	m_Capsule->SetCollisionProfileName(TEXT("Interactor"));
	m_Capsule->CanCharacterStepUpOn = ECB_No;
	m_Capsule->SetShouldUpdatePhysicsVolume(false);
	m_Capsule->SetCanEverAffectNavigation(true);
	m_Capsule->bDynamicObstacle = false;
	m_Capsule->AreaClass = nullptr;
	m_Capsule->SetMobility(EComponentMobility::Static);
	RootComponent = m_Capsule;
	m_Capsule->bReceivesDecals = false;
	
	m_InteractComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("UMyWidgetInteractionComponent"));
	m_InteractComp->SetupAttachment(m_Capsule);
	m_InteractComp->SetVisibility(true);
	
	m_InteractComp->SetDrawSize(FVector2D(90.f, 112.0f));
	m_InteractComp->SetDrawAtDesiredSize(false);
	FVector2D Pivot(0.5f, 0.5f);
	m_InteractComp->SetPivot(Pivot);
	m_InteractComp->SetWidgetSpace(EWidgetSpace::Screen);
	m_InteractComp->SetRelativeLocation(FVector(0, 0, 230));
	m_InteractComp->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void AInteractActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_InteractComp->SetWidgetClass(m_ClassInteractWidget);
}

void AInteractActorBase::OnInteract()
{
	
}

float AInteractActorBase::GetRadius() const
{
	return m_Capsule->GetScaledCapsuleRadius();
}
