// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameMode.h"

#include "MyJrpg/Actors/InitPlayerController.h"
#include "MyJrpg/Widgets/HUDs/MapHUD.h"

AMapGameMode::AMapGameMode()
{
	PlayerControllerClass = AInitPlayerController::StaticClass();
	HUDClass = AMapHUD::StaticClass();
	DefaultPawnClass = AMyPlayerPawn::StaticClass();
}

void AMapGameMode::BeginPlay()
{
	Super::BeginPlay();
}
