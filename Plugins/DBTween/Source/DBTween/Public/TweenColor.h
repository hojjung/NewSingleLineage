// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenColor.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleColor, FLinearColor, curValue, UTweenBase*, tween);

UCLASS()
class DBTWEEN_API UTweenColor : public UTweenBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "TweenUtil | TweenColor")
        static UTweenColor* DOColor(float DurTime = 1, FLinearColor Start = FLinearColor(0,0,0,1), FLinearColor End = FLinearColor(1,1,1,1),EaseType easeType = EaseType::Linear,bool IsLoop = false);

	UPROPERTY(BlueprintAssignable)
		FHandleColor OnFirst;

	UPROPERTY(BlueprintAssignable)
		FHandleColor OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FHandleColor OnComplete;

	FLinearColor StartValue;
	FLinearColor EndValue;
protected:
	virtual void UpdateFunc(float deltaTime) override;
};
