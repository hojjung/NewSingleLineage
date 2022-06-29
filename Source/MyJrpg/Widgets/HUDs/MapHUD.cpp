#include "MapHUD.h"


AMapHUD::AMapHUD()
{
	static ConstructorHelpers::FClassFinder<UWidgetMapPanel> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/Map/WB_MapPanel.WB_MapPanel_C'"));
	
	m_ClassWidget = FoundWW.Class;
}

void AMapHUD::BeginPlay()
{
	Super::BeginPlay();

	m_Canvas = CreateWidget<UWidgetMapPanel>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

UWidgetMapPanel* AMapHUD::GetCanvas()
{
	return m_Canvas;
}

void AMapHUD::PrintErrorText(const FString& str)
{
	GetCanvas()->PrintErrorText(str);
}
