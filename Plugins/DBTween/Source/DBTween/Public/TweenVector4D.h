// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenVector4D.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleVector4D, FVector4, curValue, UTweenBase*, tween);

UCLASS()
class DBTWEEN_API UTweenVector4D : public UTweenBase
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenVector4D")
        static UTweenVector4D* DOVector4D(FVector4 Start, FVector4 End, float DurTime = 1.f,EaseType easeType = EaseType::Linear,bool IsLoop = false);
	
	UPROPERTY(BlueprintAssignable)
		FHandleVector4D OnFirst;

	UPROPERTY(BlueprintAssignable)
	FHandleVector4D OnUpdate;

	UPROPERTY(BlueprintAssignable)
		FHandleVector4D OnComplete;

	FVector4 StartValue;
	FVector4 EndValue;
protected:
	virtual void UpdateFunc(float deltaTime) override;

	
};
