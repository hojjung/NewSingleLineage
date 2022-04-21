#include "WidgetConfirm.h"

#include "BUITween.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_TextInputNickname->OnTextChanged.AddDynamic(this,&UWidgetConfirm::OnTextChanged);

	m_BtnConfirm->OnClicked.AddDynamic(this, &UWidgetConfirm::OnConfirm);

	m_URL_Usage = TEXT("https://sites.google.com/d/1HlA8Ld1kt9gpu2iDYwFzINQLOfAuleM7/p/1THdXylNcaX2yBQFXU8PdHxzcTIrW8lOn/edit");

	m_URL_Private = TEXT("https://sites.google.com/d/1eNgJYa-FX0S-TKT_1oi3EwmuFPsj_Taf/p/1BQMc0eId0ROySCOxCIdI18dfaCqWvnh_/edit");

	m_URL_Refund = TEXT("https://sites.google.com/d/1v7y1DvwBQM8vmNTKL2WO-Z4-NfBZJ1hK/p/1gcJeDXzLEGjHh2KXwu7hpnNISTkGvR6o/edit");
	//Test

	UMyGameInstance::Get->StartGame();
}

void UWidgetConfirm::OnConfirm()
{
	auto CrntText =  m_TextInputNickname->GetText();

	if(CrntText.IsEmpty())
	{
		return;
	}

	if(UMyGameInstance::Get->m_BadwordTable->NicknameHasBadWord(CrntText.ToString()))
	{
		PRINTF("UWidgetConfirm::Has BadWord");
		return;
	}
	//동의 다되었는지 체크
	//나쁜 닉네임인지 체크
	//닉네임 중복되는지 체크
	UMyGameInstance::Get->StartGame();
}

FReply UWidgetConfirm::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	CheckTextTouch(InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetConfirm::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	CheckTextTouch(InMouseEvent);

	return FReply::Handled();
}

void UWidgetConfirm::CheckTextTouch(const FPointerEvent& eve)
{
	FVector2D MousePos = eve.GetScreenSpacePosition();

	if(m_TextUsage->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		OpenWebUsage();
	}
	else if(m_TextPrivate->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		OpenWebPrivate();
	}
	else if(m_TextRefund->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		OpenWebRefund();
	}
}

void UWidgetConfirm::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	UBUITween::Create( this, 0.25f )
	.FromScale(FVector2D(0,0))
	.ToScale(FVector2D(1.0f,1.0f))
	.Begin();
}

void UWidgetConfirm::OnTextChanged(const FText& text)
{
	FString Str = text.ToString();
	
	int Diff = Str.Len() - 10;
	
	if(Diff > 0)
	{
		Str = Str.LeftChop(Diff);
	}

	m_TextInputNickname->SetText(FText::FromString(Str));
}

void UWidgetConfirm::OpenWebUsage()
{
	UKismetSystemLibrary::LaunchURL(m_URL_Usage);
}

void UWidgetConfirm::OpenWebPrivate()
{
	UKismetSystemLibrary::LaunchURL(m_URL_Private);
}

void UWidgetConfirm::OpenWebRefund()
{
	UKismetSystemLibrary::LaunchURL(m_URL_Refund);
}
