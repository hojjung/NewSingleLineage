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
		m_Canvas->AddToViewport(0);
	}
}

UWidgetCanvasWorld* AMyHUD::GetCanvas()
{
	return m_Canvas;
}

void AMyHUD::PrintInfoText(const FString& str, FLinearColor color)
{
	GetCanvas()->PrintInfoText(str, color);
}

void AMyHUD::OpenItemInfo(const FItemDataRow& item_data)
{
	GetCanvas()->OpenItemInfoData(item_data);
}

