// All right Reserve 2021 HereticByte


#include "InitGameMode.h"

#include "GameFramework/PlayerState.h"
#include "MyJrpg/Actors/InitPlayerController.h"
#include "MyJrpg/Widgets/HUDs/InitHUD.h"

AInitGameMode::AInitGameMode()
{
	PlayerControllerClass=AInitPlayerController::StaticClass();
	HUDClass = AInitHUD::StaticClass();
	DefaultPawnClass=nullptr;
}

void AInitGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}
