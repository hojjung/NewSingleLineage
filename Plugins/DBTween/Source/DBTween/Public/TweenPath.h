// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenPath.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleFPath, FVector, curValue,UTweenBase*, tween);
UCLASS(BlueprintType, Blueprintable)
class DBTWEEN_API UTweenPath: public UTweenBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "TweenUtil | TweenPath")
		static UTweenPath* DOPath(TArray<FVector> Path,float DurTime = 1,EaseType easeType = EaseType::Linear,bool IsLoop = false);

	UPROPERTY(BlueprintAssignable)
		FHandleFPath OnFirst;

	UPROPERTY(BlueprintAssignable)
		FHandleFPath OnUpdate;

	UPROPERTY(BlueprintAssignable)
		FHandleFPath OnComplete;

	FVector StartValue;
	FVector CurValue;
	FVector EndValue;
	TArray<FVector> PathValue;

	float TotalLength;
	TArray<float> EachSegmentLength;


protected:
	virtual void UpdateFunc(float deltaTime) override;
	void CalcLength();
	
};
