// Copyright 2021-2022, DearBing. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TweenBase.generated.h"

UENUM(BlueprintType)
enum class EaseType : uint8
{
	Linear = 0,
	InSine = 1,
	InQuad = 2,
	InCubic = 3,
	InQuart = 4,
	InQuint = 5,
	InExpo = 6,
	InCirc = 7,
	InElastic = 8,
	InBack = 9,
	InBounce = 10,
	OutQuad = 31,
	OutSine = 32,
	OutCubic = 33,
	OutQuart = 34,
	OutQuint = 35,
	OutExpo = 36,
	OutCirc = 37,
	OutElastic = 38,
	OutBack = 39,
	OutBounce = 40,
	InOutQuad = 61,
	InOutSine = 62,
	InOutCubic = 63,
	InOutQuart = 64,
	InOutQuint = 65,
	InOutExpo = 66,
	InOutCirc = 67,
	InOutElastic = 68,
	InOutBack = 69,
	InOutBounce = 70
};

UCLASS(BlueprintType, Blueprintable)
class DBTWEEN_API UTweenBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TweenUtil | UTweenBase")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | UTweenBase")
		void Reset();

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | UTweenBase")
		void Play();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TweenUtil | UTweenBase")
		bool Paused = false;

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | UTweenBase")
		void SetOvershootOrAmplitudeAndPeriod(float OvershootOrAmplitudeParam = 1.70158f, float PeriodParam = 0);

	float easePeriod;
	float easeOvershootOrAmplitude = 1.70158f;

	void KillTween();
	
protected:

	void Init(float durTime);
	virtual void UpdateFunc(float deltaTime);
	void SetLooping(bool bIsLoop);

	virtual void SetReadyToDestroy() override;
	float TotalTime;
	float CurTime;
	int UpdateKey;
	bool Finished;
	bool bLoop = false;
	bool bReverse = false;
	bool isInit = false;
	EaseType CurEasyType = EaseType::Linear;
	float Evaluate();
	float BounceEaseIn(float time, float duration, float unusedOvershootOrAmplitude, float unusedPeriod);
	float BounceEaseInOut(float time, float duration, float unusedOvershootOrAmplitude, float unusedPeriod);
	float BounceEaseOut(float time, float duration, float unusedOvershootOrAmplitude, float unusedPeriod);
};
