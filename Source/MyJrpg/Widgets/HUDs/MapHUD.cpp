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

void AMapHUD::PrintInfoText(const FString& str, FLinearColor color)
{
	GetCanvas()->PrintInfoText(str, color);
}

void AMapHUD::OpenItemInfo(const FItemDataRow& item_data)
{
	GetCanvas()->OpenItemInfoData(item_data);
}

void AMapHUD::SetConfirmPanel(const FText& txt, UWidgetConfirmPanel::FOnClick onCancel,
	UWidgetConfirmPanel::FOnClick onConfirm)
{
	m_Canvas->GetConfirmPanel()->SetConfirmPanel(txt,onCancel,onConfirm);
}
