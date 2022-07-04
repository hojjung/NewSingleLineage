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
