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
	SetHiddenInGame(true);
}

void UInGameTextWidgetComp::SetTextWant(const FText& textWant, ETextType dmg)
{
	SetHiddenInGame(false);
	
	UInGameTextWidget* InGameWidget =  Cast<UInGameTextWidget>( GetUserWidgetObject());
	
	InGameWidget->SetTextWant(textWant,dmg);
	
	InGameWidget->m_OnAnimEnd.BindUObject(this,&UInGameTextWidgetComp::EndAnimation);
	
	RequestRedraw();
}

void UInGameTextWidgetComp::EndAnimation()
{
	SetHiddenInGame(true);
	SetRelativeLocation(FVector(0,0,0));
}