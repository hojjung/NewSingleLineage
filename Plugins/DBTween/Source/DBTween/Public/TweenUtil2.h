// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TweenActor.h"
#include "TweenCanvasPanelSlot.h"
#include "TweenColor.h"
#include "TweenFloat.h"
#include "TweenPath.h"
#include "TweenVector2D.h"
#include "TweenVector3D.h"
#include "TweenVector4D.h"
#include "TweenWidget.h"
#include "TweenUtil2.generated.h"

/**
 * 
 */
UCLASS()
class DBTWEEN_API UTweenUtil2 : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenCanvasPanelSlot")
		static UTweenCanvasPanelSlot* DOMoveSync(float DurTime = 1, UCanvasPanelSlot* CanvasPanelSlot = nullptr, FVector2D End = FVector2D(100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenCanvasPanelSlot")
		static UTweenCanvasPanelSlot* DOSizeSync(float DurTime = 1, UCanvasPanelSlot* CanvasPanelSlot = nullptr, FVector2D End = FVector2D(100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenWidget")
		static UTweenWidget* DOLocalMoveSync(float DurTime = 1, UWidget* UI = nullptr, FVector2D End = FVector2D(100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenWidget")
		static UTweenWidget* DOLocalAngleSync(float DurTime = 1, UWidget* UI = nullptr, float End = 90, EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenWidget")
		static UTweenWidget* DOLocalScaleSync(float DurTime = 1, UWidget* UI = nullptr, FVector2D End = FVector2D(2, 2), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenWidget")
		static UTweenWidget* DOFadeSync(float DurTime = 1, UWidget* UI = nullptr, float End = 1, EaseType easeType = EaseType::Linear, bool IsLoop = false);
};
