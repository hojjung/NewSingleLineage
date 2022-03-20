// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenWidget.h"

#include "Kismet/KismetMathLibrary.h"


UTweenWidget* UTweenWidget::DOLocalMove(float DurTime, UWidget* UI, FVector2D End, EaseType easeType, bool IsLoop)
{
	if(!UI) return nullptr;
	UTweenWidget* result = NewObject<UTweenWidget>();
	result->Init(DurTime);
	result->StartTranslationValue = UI->RenderTransform.Translation;
	result->EndTranslationValue = End;
	result->SetLooping(IsLoop);
	result->Widget = UI;
	result->TweenWidgetType = ETweenWidgetType::E_WidgetTranslation;
	result->CurEasyType = easeType;
	return result;
}

UTweenWidget* UTweenWidget::DOLocalAngle(float DurTime, UWidget* UI, float End, EaseType easeType, bool IsLoop)
{
	if(!UI) return nullptr;
	UTweenWidget* result = NewObject<UTweenWidget>();
	result->Init(DurTime);
	result->StartAngleValue = UI->GetRenderTransformAngle();
	result->EndAngleValue = End;
	result->SetLooping(IsLoop);
	result->Widget = UI;
	result->TweenWidgetType = ETweenWidgetType::E_WidgetAngle;
	result->CurEasyType = easeType;
	return result;
}

UTweenWidget* UTweenWidget::DOLocalScale(float DurTime, UWidget* UI, FVector2D End, EaseType easeType, bool IsLoop)
{
	if(!UI) return nullptr;
	UTweenWidget* result = NewObject<UTweenWidget>();
	result->Init(DurTime);
	result->StartScaleValue = UI->RenderTransform.Scale;
	result->EndScaleValue = End;
	result->SetLooping(IsLoop);
	result->Widget = UI;
	result->TweenWidgetType = ETweenWidgetType::E_WidgetScale;
	result->CurEasyType = easeType;
	return result;
}

UTweenWidget* UTweenWidget::DOFade(float DurTime, UWidget* UI, float End, EaseType easeType, bool IsLoop)
{
	if(!UI) return nullptr;
	UTweenWidget* result = NewObject<UTweenWidget>();
	result->Init(DurTime);
	result->StartOpacityValue = UI->GetRenderOpacity();
	result->EndOpacityValue = End;
	result->SetLooping(IsLoop);
	result->Widget = UI;
	result->TweenWidgetType = ETweenWidgetType::E_WidgetFade;
	result->CurEasyType = easeType;
	return result;
}

void UTweenWidget::UpdateFunc(float deltaTime)
{
	if (Paused)return;
	Super::UpdateFunc(deltaTime);
	if (!Widget.Get()) return;
	if (!isInit)
	{
		isInit = true;
		OnFirst.Broadcast(this);
	}
	switch (TweenWidgetType)
	{
		case E_WidgetTranslation:
			{
				FVector2D CurValue = StartTranslationValue +  Evaluate() * (EndTranslationValue - StartTranslationValue);
				Widget->SetRenderTranslation(CurValue);
			}
			break;
		case E_WidgetAngle:
			{
				float CurValue = StartAngleValue +  Evaluate() * (EndAngleValue - StartAngleValue);
				Widget->SetRenderTransformAngle(CurValue);
			}
			break;
		case E_WidgetScale:
			{
				FVector2D CurValue = StartScaleValue +  Evaluate() * (EndScaleValue - StartScaleValue);
				Widget->SetRenderScale(CurValue);
			}
			break;
		case E_WidgetFade:
			{
				float CurValue = StartOpacityValue +  Evaluate() * (EndOpacityValue - StartOpacityValue);
				Widget->SetRenderOpacity(CurValue);
			}
			break;
	}
	
	OnUpdate.Broadcast(this);
	if (Finished)
	{
		OnComplete.Broadcast(this);
	}

}

