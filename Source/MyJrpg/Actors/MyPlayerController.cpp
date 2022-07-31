#include "MyPlayerController.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyCheatManager.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidget.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetConfirmPanel.h"

AMyPlayerController::AMyPlayerController()
{
	CheatClass = UMyCheatManager::StaticClass();
	
	m_CompUseIndex = 0;

	bShowMouseCursor = true;
	//
	SetHidden(false);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CreateIGWC(10);

	SetVirtualJoystickVisibility(true);

	m_Joystick = CurrentTouchInterface;
}

void AMyPlayerController::CreateIGWC(int count)
{
	m_AryIGWC.Reset();

	for (int i = 0; i < count; i++)
	{
		UInGameTextWidgetComp* DamageText = NewObject<UInGameTextWidgetComp>(this, UInGameTextWidgetComp::StaticClass());
		DamageText->RegisterComponent();
		DamageText->InitializeComponent();
		m_AryIGWC.Add(DamageText);
		DamageText->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

UInGameTextWidgetComp* AMyPlayerController::GetIGWC()
{
	UInGameTextWidgetComp* Dmg = m_AryIGWC[m_CompUseIndex++];

	if (m_CompUseIndex >= m_AryIGWC.Num())
	{
		m_CompUseIndex = 0;
	}
	return Dmg;
}

void AMyPlayerController::ShowInGameWorldText(const FString& stringWant, const ABaseUnitPawn* interactActor,
                                              ETextType dmgPopup)
{
	UInGameTextWidgetComp* TextCompWant = GetIGWC();

	TextCompWant->SetTextWant(interactActor->GetActorLocation(), FText::FromString(stringWant), dmgPopup);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &AMyPlayerController::OpenExitPanel);
	InputComponent->BindAction("MouseClick", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnTouchPressed);
}

void AMyPlayerController::OpenExitPanel()
{
	UWidgetConfirmPanel::FOnClick Cancel;

	UWidgetConfirmPanel::FOnClick Confirm = UWidgetConfirmPanel::FOnClick::CreateUObject(this,&AMyPlayerController::ExitGame); 

	FString DescStrF = NSLOCTEXT("AMyPlayerController","OnExit","정말로 게임을 종료 하시겠습니까?").ToString();

	UMyLib::SetConfirmPanel(DescStrF,Cancel,Confirm);
}

void AMyPlayerController::ShowInGameWorldText(float number, ABaseUnitPawn* interactActor, ETextType dmgPopup)
{
	FString Number =  FString::Printf(TEXT("%.0f"),number);
	ShowInGameWorldText(Number,interactActor,dmgPopup);
}

void AMyPlayerController::BackToSelectMenu()
{
	PRINTF("Continue StartMenu");
	ClientForceGarbageCollection();
	UGameplayStatics::OpenLevel(GetWorld(), "StartMenu");
}

void AMyPlayerController::ExitGame()
{
	PRINTF("TryExit");

	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void AMyPlayerController::OnTouchPressed()
{
	FHitResult Hit;
    	
	GetHitResultUnderCursor(ECC_EngineTraceChannel4, false, Hit);
    
	if(!Hit.bBlockingHit)
	{
		return ;
	}

	m_OnTouch.Broadcast(Hit);
}

void AMyPlayerController::EnableJoystick(bool b)
{
	if(b)
	{
		ActivateTouchInterface(m_Joystick);
	}
	else
	{
		ActivateTouchInterface(nullptr);
	}
}
