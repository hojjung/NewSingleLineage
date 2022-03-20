// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenFloat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleFloat, float, curValue, UTweenBase*, tween);
UCLASS()
class DBTWEEN_API UTweenFloat : public UTweenBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "TweenUtil | TweenFloat")
		static UTweenFloat* DOFloat(float DurTime = 1, float Start = 0, float End = 1, EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UPROPERTY(BlueprintAssignable)
		FHandleFloat OnFirst;

	UPROPERTY(BlueprintAssignable)
		FHandleFloat OnUpdate;

	UPROPERTY(BlueprintAssignable)
		FHandleFloat OnComplete;

	float StartValue;
	float EndValue;
protected:
	virtual void UpdateFunc(float deltaTime) override;
};
