// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenVector3D.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleVector3D, FVector, curValue, UTweenBase*, tween);

UCLASS()
class DBTWEEN_API UTweenVector3D : public UTweenBase
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenVector3D")
	static UTweenVector3D* DOVector3D(float DurTime = 1.f, FVector Start = FVector(0,0,0), FVector End = FVector(100,100,100), EaseType easeType = EaseType::Linear, bool IsLoop = false);
		
	UPROPERTY(BlueprintAssignable)
		FHandleVector3D OnFirst;

	UPROPERTY(BlueprintAssignable)
	FHandleVector3D OnUpdate;

	UPROPERTY(BlueprintAssignable)
		FHandleVector3D OnComplete;

	FVector StartValue;
	FVector EndValue;
	protected:
	virtual void UpdateFunc(float deltaTime) override;
	
};
