// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void PrintErrorText(const FString& str);

	void PrintErrorText(FText&& tt);
};



