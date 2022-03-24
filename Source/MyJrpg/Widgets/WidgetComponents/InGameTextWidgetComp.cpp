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
	SetManuallyRedraw(true);
}

void UInGameTextWidgetComp::BeginPlay()
{
	Super::BeginPlay();
	SetWidgetClass(m_ClassTextWidget);
	m_TextWidget = Cast<UInGameTextWidget>(GetUserWidgetObject()); 
	m_TextWidget->SetParentComponent(this);
	
}

void UInGameTextWidgetComp::SetTextWant(FVector loc,const FText& textWant, ETextType dmg)
{
	SetWorldLocation(loc);

	m_TextWidget->SetTextWant(textWant,dmg);
}