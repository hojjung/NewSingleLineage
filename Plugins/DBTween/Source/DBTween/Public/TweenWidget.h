// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "Components/Widget.h"
#include "TweenWidget.generated.h"

/**
 * 
 */

enum ETweenWidgetType
{
    E_WidgetTranslation,
    E_WidgetAngle,
    E_WidgetScale,
	E_WidgetFade
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandleWidget, UTweenBase*, tween);

UCLASS()
class DBTWEEN_API UTweenWidget : public UTweenBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenWidget")
        static UTweenWidget* DOLocalMove(float DurTime = 1, UWidget* UI = nullptr, FVector2D End = FVector2D(100,100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "TweenUtil | TweenWidget")
		static UTweenWidget* DOLocalAngle(float DurTime = 1, UWidget* UI = nullptr, float End = 90, EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenWidget")
        static UTweenWidget* DOLocalScale(float DurTime = 1, UWidget* UI = nullptr, FVector2D End = FVector2D(2,2), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenWidget")
        static UTweenWidget* DOFade(float DurTime = 1, UWidget* UI = nullptr, float End = 1, EaseType easeType = EaseType::Linear, bool IsLoop = false);
	

	UPROPERTY(BlueprintAssignable)
		FHandleWidget OnFirst;

	UPROPERTY(BlueprintAssignable)
		FHandleWidget OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FHandleWidget OnComplete;

	FVector2D StartTranslationValue;
	FVector2D EndTranslationValue;

	float StartAngleValue;
	float EndAngleValue;

	FVector2D StartScaleValue;
	FVector2D EndScaleValue;

	float StartOpacityValue;
	float EndOpacityValue;

protected:
	virtual void UpdateFunc(float deltaTime) override;
	TWeakObjectPtr<UWidget> Widget;
	ETweenWidgetType TweenWidgetType = ETweenWidgetType::E_WidgetTranslation;
};
