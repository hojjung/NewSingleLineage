// All right Reserve 2021 HereticByte


#include "InitPlayerController.h"

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyCheatManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetConfirmPanel.h"

AInitPlayerController::AInitPlayerController()
{
	bShowMouseCursor=true;

	CheatClass = UMyCheatManager::StaticClass();
}

void AInitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &AInitPlayerController::OnExitGame);
}

void AInitPlayerController::OnExitGame()
{
	UWidgetConfirmPanel::FOnClick Cancel;

	UWidgetConfirmPanel::FOnClick Confirm = UWidgetConfirmPanel::FOnClick::CreateUObject(this,&AInitPlayerController::ExitGame); 

	FString DescStrF = NSLOCTEXT("AMyPlayerController","OnExit","정말로 게임을 종료 하시겠습니까?").ToString();

	UMyLib::SetConfirmPanel(DescStrF,Cancel,Confirm);
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
