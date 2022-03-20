// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "Components/CanvasPanelSlot.h"
#include "TweenCanvasPanelSlot.generated.h"

/**
 * 
 */
enum ETweenCanvasSlotType
{
    E_CanvasSlotPos,
    E_CanvasSlotSize
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandleCanvasPanel, UTweenBase*, tween);

UCLASS()
class DBTWEEN_API UTweenCanvasPanelSlot : public UTweenBase
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenCanvasPanelSlot")
        static UTweenCanvasPanelSlot* DOMove(float DurTime = 1, UCanvasPanelSlot* CanvasPanelSlot = nullptr, FVector2D End = FVector2D(100,100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenCanvasPanelSlot")
        static UTweenCanvasPanelSlot* DOSize(float DurTime = 1, UCanvasPanelSlot* CanvasPanelSlot = nullptr, FVector2D End = FVector2D(100,100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UPROPERTY(BlueprintAssignable)
	FHandleCanvasPanel OnFirst;

	UPROPERTY(BlueprintAssignable)
	FHandleCanvasPanel OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FHandleCanvasPanel OnComplete;

	FVector2D StartValue;
	FVector2D EndValue;
	
protected:
	virtual void UpdateFunc(float deltaTime) override;
	TWeakObjectPtr<UCanvasPanelSlot> Slot;
	ETweenCanvasSlotType TweenCanvasSlotType = ETweenCanvasSlotType::E_CanvasSlotPos;
};
