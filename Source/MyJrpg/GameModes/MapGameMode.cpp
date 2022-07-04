// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameMode.h"

#include "MyJrpg/Actors/InitPlayerController.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/HUDs/MapHUD.h"

AMapGameMode::AMapGameMode()
{
	PlayerControllerClass = AInitPlayerController::StaticClass();
	HUDClass = AMapHUD::StaticClass();
	DefaultPawnClass = AMyPlayerPawn::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void AMapGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMapGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UMyGameInstance::Get->m_ZoneMove->Tick(DeltaSeconds);
}
