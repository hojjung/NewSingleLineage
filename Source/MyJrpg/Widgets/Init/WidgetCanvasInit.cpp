#include "WidgetCanvasInit.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCanvasInit::PrintInfoText(const FString& str, FLinearColor color)
{
	m_AlertInfoWindow->PrintInfoText(str, color);
}

void UWidgetCanvasInit::OnCloseNews()
{
	m_NewsCanvas->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetCanvasInit::StartPlayfabLogin()
{
	m_fTimer = 0.f;
	
	m_ImgBlink->SetVisibility(ESlateVisibility::Collapsed);
	
	m_WidgetConfirm->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCloseNews->OnClicked.AddDynamic(this, &UWidgetCanvasInit::OnCloseNews);

	UMyGameInstance::Get->m_PlayfabManager->RequestTitleNews(FNewsDele::CreateUObject(this, &UWidgetCanvasInit::OnSuccessGetTitleNews));

	UMyGameInstance::Get->m_PlayfabManager->StartPlayfabLogin(UPlayfabManager::FOnLoginEnd::CreateUObject(this, &UWidgetCanvasInit::OpenConfirmPanel));

	UMyGameInstance::Get->m_PlayfabManager->m_OnNickNameFail.BindUObject(this, &UWidgetCanvasInit::OpenConfirmPanel);
}

void UWidgetCanvasInit::OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt)
{
	TArray<PlayFab::ClientModels::FTitleNewsItem> NewsArray = rslt.News;

	FTimespan KoreanTime(9,0,0);
	
	FDateTime Time = NewsArray[0].Timestamp + KoreanTime;

	FString Title = FString::Printf(TEXT("%s-%s"), *NewsArray[0].Title, *Time.ToString());
	
	m_TextTitle->SetText(FText::FromString(Title));
	
	m_TextTitleNews->SetText(FText::FromString(NewsArray[0].Title));

	m_TextTitleNews->SetText(FText::FromString(NewsArray[0].Body));
}

void UWidgetCanvasInit::OpenConfirmPanel()
{
	m_WidgetConfirm->Show();
}

FReply UWidgetCanvasInit::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	const FString& Nick = UMyGameInstance::Get->m_PlayfabManager->GetNickName();
	
	if(Nick.IsEmpty() || m_NewsCanvas->IsVisible())
	{
		return FReply::Handled(); 
	}
	UMyGameInstance::Get->StartGame();

	return FReply::Handled(); 
}

FReply UWidgetCanvasInit::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const FString& Nick = UMyGameInstance::Get->m_PlayfabManager->GetNickName();
	
	if(Nick.IsEmpty() || m_NewsCanvas->IsVisible())
	{
		return FReply::Handled(); 
	}
	UMyGameInstance::Get->StartGame();

	return FReply::Handled();
}

void UWidgetCanvasInit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FString& Nick = UMyGameInstance::Get->m_PlayfabManager->GetNickName();

	if(Nick.IsEmpty())
	{
		return ; 
	}

	m_fTimer += InDeltaTime;

	if (m_fTimer >=0.15f)
	{
		if(m_ImgBlink->IsVisible())
		{
			m_ImgBlink->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			m_ImgBlink->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		m_fTimer = 0;
	}
}
