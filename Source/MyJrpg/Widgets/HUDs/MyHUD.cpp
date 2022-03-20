#include "MyHUD.h"

#include "UObject/ConstructorHelpers.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UWidgetCanvasWorld> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/WB_WorldCanvas.WB_WorldCanvas_C'"));
	
	m_ClassWidget = FoundWW.Class;
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	m_Canvas = CreateWidget<UWidgetCanvasWorld>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

UWidgetCanvasWorld* AMyHUD::GetCanvas()
{
	return m_Canvas;
}

