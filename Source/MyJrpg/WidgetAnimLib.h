// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUITweenInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetAnimLib.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetAnimLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void PlayTextAnim(UTextBlock* textblock, FLinearColor colorWant, float dur);
	
	static FBUITweenInstance& PlayAlphaFlashAnim(UImage* textblock, float dur);

protected:
	static void RecoverTextAnim(UTextBlock* textblock, float dur);

	static void RecoverAlphaFlashAnim(UImage* textblock, float dur);
};
