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
#include "TweenUtil.generated.h"

/**
 * 
 */
UCLASS()
class DBTWEEN_API UTweenUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category = "TweenUtil | TweenActor")
		static UTweenActor* DOLocalMoveSync(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(100, 100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable,Category = "TweenUtil | TweenActor")
		static UTweenActor* DOLocalRotationSync(float DurTime = 1.f, AActor* Actor = nullptr, FRotator End = FRotator(0, 90, 0), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenActor")
		static UTweenActor* DOLocalScaleSync(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(2, 2, 2), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenActor")
		static UTweenActor* DOMoveSync(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(100, 100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenActor")
		static UTweenActor* DORotationSync(float DurTime = 1.f, AActor* Actor = nullptr, FRotator End = FRotator(0, 90, 0), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenActor")
		static UTweenActor* DOScaleSync(float DurTime = 1.f, AActor* Actor = nullptr, FVector End = FVector(2, 2, 2), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenColor")
		static UTweenColor* DOColorSync(float DurTime = 1, FLinearColor Start = FLinearColor(0, 0, 0, 1), FLinearColor End = FLinearColor(1, 1, 1, 1), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenFloat")
		static UTweenFloat* DOFloatSync(float DurTime = 1, float Start = 0, float End = 1, EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenPath")
		static UTweenPath* DOPathSync(TArray<FVector> Path, float DurTime = 1, EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenPath")
		static UTweenPath* DOPathJumpSync(FVector start, FVector end, float height, float DurTime = 1, EaseType easeType = EaseType::Linear);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenVector2D")
		static UTweenVector2D* DOVector2DSync(float DurTime = 1, FVector2D Start = FVector2D(0, 0), FVector2D End = FVector2D(100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenVector3D")
		static UTweenVector3D* DOVector3DSync(float DurTime = 1.f, FVector Start = FVector(0, 0, 0), FVector End = FVector(100, 100, 100), EaseType easeType = EaseType::Linear, bool IsLoop = false);

	UFUNCTION(BlueprintCallable, Category = "TweenUtil | TweenVector4D")
		static UTweenVector4D* DOVector4DSync(FVector4 Start, FVector4 End, float DurTime = 1.f, EaseType easeType = EaseType::Linear, bool IsLoop = false);
};
