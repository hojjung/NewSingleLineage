// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TweenBase.h"
#include "TweenActor.generated.h"

/**
 * 
 */
enum ETweenActorType
{
	E_ActorRelativeLocation,
    E_ActorRelativeRotation,
    E_ActorRelativeScale,
	E_ActorLocation,
	E_ActorRotation,
	E_ActorScale
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandleActor, UTweenBase*,tween);

UCLASS()
class DBTWEEN_API UTweenActor : public UTweenBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "TweenUtil | TweenActor")
		static UTweenActor* DOLocalMove(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(100,100,100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenActor")
        static UTweenActor* DOLocalRotation(float DurTime = 1.f, AActor* Actor = nullptr, FRotator End = FRotator(0,90	,0), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenActor")
        static UTweenActor* DOLocalScale(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(2,2,2), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),Category = "TweenUtil | TweenActor")
        static UTweenActor* DOMove(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(100,100,100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenActor")
        static UTweenActor* DORotation(float DurTime = 1.f, AActor* Actor = nullptr, FRotator End = FRotator(0,90	,0), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"), Category = "TweenUtil | TweenActor")
        static UTweenActor* DOScale(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(2,2,2), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UPROPERTY(BlueprintAssignable)
		FHandleActor OnFirst;

	UPROPERTY(BlueprintAssignable)
		FHandleActor OnUpdate;

	UPROPERTY(BlueprintAssignable)
		FHandleActor OnComplete;

	FVector StartLocValue;
	FVector EndLocValue;

	FQuat StartRotValue;
	FQuat EndRotValue;

	FVector StartScaleValue;
	FVector EndScaleValue;

protected:
	virtual void UpdateFunc(float deltaTime) override;

	TWeakObjectPtr<AActor> Actor;
	ETweenActorType TweenActorType = ETweenActorType::E_ActorLocation;
};
