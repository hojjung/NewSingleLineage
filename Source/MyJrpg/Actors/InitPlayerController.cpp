// All right Reserve 2021 HereticByte


#include "InitPlayerController.h"

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Managers/MyCheatManager.h"

AInitPlayerController::AInitPlayerController()
{
	bShowMouseCursor=true;

	CheatClass = UMyCheatManager::StaticClass();
}

void AInitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &AInitPlayerController::ExitGame);
	InputComponent->BindAction("AndroidBack", EInputEvent::IE_Pressed, this, &AInitPlayerController::ExitGame);
}


void AInitPlayerController::ExitGame()
{
	PRINTF("TryExit");

	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void AInitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	APlayerController::SetVirtualJoystickVisibility(false);
}
