// All right Reserve 2021 HereticByte


#include "WidgetCanvasInit.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCanvasInit::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_WidgetConfirm->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UWidgetCanvasInit::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	GameStart();

	return FReply::Handled();
}

FReply UWidgetCanvasInit::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	GameStart();

	return FReply::Handled();
}

void UWidgetCanvasInit::GameStart()
{
	//bool if account created

	if(!m_WidgetConfirm->IsVisible())
	{
		m_WidgetConfirm->Show();
	}
}

void UWidgetCanvasInit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	m_fTimer += InDeltaTime;

	if (m_fTimer >=0.15f)
	{
		if(m_ImgBlinkText->IsVisible())
		{
			m_ImgBlinkText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			m_ImgBlinkText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		m_fTimer = 0;
	}
}
