#include "MyPlayerController.h"
#include "NavigationSystem.h"
#include "Field/InteractActorBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyCheatManager.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidget.h"

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

	FActorSpawnParameters Param;

	Param.bNoFail = true;
	
	m_FocusActor = GetWorld()->SpawnActor<AMoveIndicator>(AMoveIndicator::StaticClass(),FVector(0,0,0),FRotator(0,0,0),Param);
	m_FocusActor->SetActorHiddenInGame(true);

	SetVirtualJoystickVisibility(true);

	m_Joystick = CurrentTouchInterface;

	GetPawn<AMyPlayerPawn>()->m_OnFocus.AddUObject(this, &AMyPlayerController::OnFocus);
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
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &AMyPlayerController::ExitGame);
	InputComponent->BindAction("MouseClick", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnTouchPressed);
}

bool AMyPlayerController::CheckInteract()
{
	FHitResult Hit;
    	
    GetHitResultUnderCursor(ECC_EngineTraceChannel4, false, Hit);
    
    if(!Hit.bBlockingHit)
    {
    	return false;
    }

	m_OnTouch.Broadcast(Hit);

	return true;
}

void AMyPlayerController::OnFocus(IFocusable* focus)
{
	if(!focus)
	{
		m_FocusActor->SetActorHiddenInGame(true);
		return;
	}
	
	m_FocusActor->SetActorHiddenInGame(false);

	AActor* FocusActor = Cast<AActor>(focus);

	FVector Loc = FocusActor->GetActorLocation();
	
	float H =  focus->GetBoundHalfHeight();

	Loc.Z -= H;
	
	FAttachmentTransformRules Rule(EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,false);
	
	m_FocusActor->AttachToActor(FocusActor, Rule);

	m_FocusActor->SetActorLocation(Loc);
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
	CheckInteract();
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
