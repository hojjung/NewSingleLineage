// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActorBase.h"

#include "Engine/CollisionProfile.h"
#include "MyJrpg/MyJrpg.h"

// Sets default values
AInteractActorBase::AInteractActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule00"));
	m_Capsule->InitCapsuleSize(34.0f, 88.0f);
	m_Capsule->SetCollisionProfileName(TEXT("BlockAll"));
	m_Capsule->CanCharacterStepUpOn = ECB_No;
	m_Capsule->SetShouldUpdatePhysicsVolume(false);
	m_Capsule->SetCanEverAffectNavigation(true);
	m_Capsule->bDynamicObstacle = false;
	m_Capsule->AreaClass = nullptr;
	m_Capsule->SetMobility(EComponentMobility::Movable);
	m_Capsule->bReceivesDecals = false;
	m_Capsule->SetupAttachment(RootComponent);
}

void AInteractActorBase::OnInteract()
{
	
}

float AInteractActorBase::GetBoundHalfHeight()
{
	return m_Capsule->GetScaledCapsuleHalfHeight();
}
