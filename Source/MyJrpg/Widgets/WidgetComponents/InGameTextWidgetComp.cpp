#include "InGameTextWidgetComp.h"

#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidget.h"
#include "UObject/ConstructorHelpers.h"

UInGameTextWidgetComp::UInGameTextWidgetComp()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
	TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/WB_PopText.WB_PopText_C'"));
    
	m_ClassTextWidget = FoundW.Class;

	SetDrawAtDesiredSize(true);
	SetPivot(FVector2D (0.5f, 0.5f));
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCanEverAffectNavigation(false);
}

void UInGameTextWidgetComp::BeginPlay()
{
	Super::BeginPlay();
	SetWidgetClass(m_ClassTextWidget);
	Cast<UInGameTextWidget>(GetUserWidgetObject())->SetParentComponent(this);
	SetHiddenInGame(true);
}

void UInGameTextWidgetComp::SetTextWant(const FText& textWant, ETextType dmg)
{
	UInGameTextWidget* InGameWidget =  Cast<UInGameTextWidget>( GetUserWidgetObject());

	InGameWidget->SetTextWant(textWant,dmg);
	
	RequestRedraw();
}