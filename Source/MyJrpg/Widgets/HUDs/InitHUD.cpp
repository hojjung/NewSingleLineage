#include "InitHUD.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/Init/WidgetCanvasInit.h"
#include "UObject/ConstructorHelpers.h"

AInitHUD::AInitHUD()
{
	static ConstructorHelpers::FClassFinder<UWidgetCanvasInit> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/Init/WB_InitCanvas.WB_InitCanvas_C'"));
	
	m_ClassWidget = FoundWW.Class;	
}

void AInitHUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UWidgetCanvasInit>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
	
	m_Canvas->StartPlayfabLogin();
}

void AInitHUD::PrintInfoText(const FString& str, FLinearColor color)
{
	m_Canvas->PrintInfoText(str, color);
}
