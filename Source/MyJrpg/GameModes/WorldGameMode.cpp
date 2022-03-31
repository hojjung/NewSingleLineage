#include "WorldGameMode.h"
#include "MyJrpg/Actors/MyPlayerController.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/PetManager.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Widgets/HUDs/MyHUD.h"

AWorldGameMode::AWorldGameMode()
{
	PlayerControllerClass=AMyPlayerController::StaticClass();
	HUDClass = AMyHUD::StaticClass();
	DefaultPawnClass=AMyPlayerPawn::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void AWorldGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UMyGameInstance::Get->Tick(DeltaSeconds);
}

void AWorldGameMode::StartPlay()
{
	UMyGameInstance::Get->m_AvatarManager->CreatePreviewActor();
	UMyGameInstance::Get->m_PetManager->CreatePreviewActor();	
	Super::StartPlay();
}
