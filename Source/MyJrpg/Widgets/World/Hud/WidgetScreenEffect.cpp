// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetScreenEffect.h"

#include "BUITween.h"
#include "MyJrpg/WidgetAnimLib.h"

void UWidgetScreenEffect::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_ImgTakeHit->SetRenderOpacity(0.f);
	m_ImgTakeHit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	EndFade();
}

void UWidgetScreenEffect::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_fMaxFadeOut <= -1)
	{
		return;
	}

	if(IsFadeOut)
	{
		m_fFadeOutTimer += InDeltaTime;
	}
	else
	{
		m_fFadeOutTimer -= InDeltaTime;
	}

	float Per = m_fFadeOutTimer / m_fMaxFadeOut;
	
	m_ImgBlack->SetRenderOpacity(Per);

	if(m_fFadeOutTimer >= m_fMaxFadeOut)
	{
		m_OnFadeOutDone.ExecuteIfBound();
		//EndFade();
	}
}

void UWidgetScreenEffect::EndFade()
{
	m_ImgBlack->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ImgBlack->SetRenderOpacity(0);
	
	m_fMaxFadeOut = -1;

	m_fFadeOutTimer = -1;
	
	IsFadeOut = false;
	
	m_OnFadeOutDone.Unbind();
}

void UWidgetScreenEffect::ShowFadeOut(float t, const FVoidVoid& onFadeOutEnd)
{
	if(IsFadeOut)
	{
		return;
	}

	IsFadeOut = true;
	
	m_ImgBlack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_fFadeOutTimer = 0;
	
	m_fMaxFadeOut = t;

	m_OnFadeOutDone = onFadeOutEnd;
}

void UWidgetScreenEffect::ShowHittenFlash()
{
	if(UBUITween::GetIsTweening(m_ImgTakeHit))
	{
		return;
	}
	UWidgetAnimLib::PlayAlphaFlashAnim(m_ImgTakeHit, 1.f);
}

void UWidgetScreenEffect::HideFadeOut()
{
	if(!IsFadeOut)
	{
		return;
	}
	EndFade();
}
