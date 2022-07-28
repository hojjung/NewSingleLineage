// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetAnimLib.h"

#include "BUITween.h"

void UWidgetAnimLib::PlayTextAnim(UTextBlock* textblock, FLinearColor colorWant, float dur)
{
	textblock->SetColorAndOpacity(colorWant);
	UBUITween::Create(textblock, dur)
		.FromScale(FVector2D(1.0f, 1.0f))
		.ToScale(FVector2D(1.5f, 1.5f))
		.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
		{
			UTextBlock* OwnerT = Cast<UTextBlock>(Owner);
			if(OwnerT)
			{
				RecoverTextAnim(OwnerT, 0.2f);
			}
		}))
		.Begin();
}



void UWidgetAnimLib::RecoverTextAnim(UTextBlock* textblock, float dur)
{
	textblock->SetColorAndOpacity(FLinearColor::White);
	UBUITween::Create(textblock, dur)
		.FromScale(FVector2D(1.5f, 1.5f))
		.ToScale(FVector2D(1.0f, 1.0f))
		.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
		{
			
		}))
		.Begin();
}

FBUITweenInstance& UWidgetAnimLib::PlayAlphaFlashAnim(UImage* textblock, float dur)
{
	float Dur1 = dur * 0.2f;
	float Dur2 = dur * 0.8f;
	
	FBUITweenInstance& Tween = UBUITween::Create(textblock, Dur1)
	                        .FromOpacity(0.f)
	                        .ToOpacity(1.f)
	                        .OnComplete(FBUITweenSignature::CreateLambda([=](UWidget* Owner)
	                        {
		                        UImage* OwnerT = Cast<UImage>(Owner);
		                        if (OwnerT)
		                        {
			                        RecoverAlphaFlashAnim(OwnerT, Dur2);
		                        }
	                        }));

	Tween.Begin();
	return Tween;
}

void UWidgetAnimLib::RecoverAlphaFlashAnim(UImage* textblock, float dur)
{
	UBUITween::Create(textblock, dur)
		.FromOpacity(1.f)
		.ToOpacity(0.f)
		
	.Begin();
}
