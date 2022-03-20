// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenVector2D.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleVector2D, FVector2D, curValue, UTweenBase*, tween);

UCLASS()
class DBTWEEN_API UTweenVector2D : public UTweenBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenVector2D")
        static UTweenVector2D* DOVector2D(float DurTime = 1, FVector2D Start = FVector2D(0,0), FVector2D End = FVector2D(100,100),EaseType easeType = EaseType::Linear,bool IsLoop = false);

	UPROPERTY(BlueprintAssignable)
		FHandleVector2D OnFirst;
	
	UPROPERTY(BlueprintAssignable)
	FHandleVector2D OnUpdate;

	UPROPERTY(BlueprintAssignable)
		FHandleVector2D OnComplete;

	FVector2D StartValue;
	FVector2D EndValue;
protected:
	virtual void UpdateFunc(float deltaTime) override;
};
