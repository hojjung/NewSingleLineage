// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseHUD.h"
#include "GameFramework/HUD.h"
#include "MyJrpg/Widgets/MapSelect/WidgetMapPanel.h"
#include "MyJrpg/Widgets/World/WidgetCanvasWorld.h"
#include "MapHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AMapHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	AMapHUD();

protected:
	UPROPERTY()
	TSubclassOf<UWidgetMapPanel> m_ClassWidget;
	UPROPERTY()
	UWidgetMapPanel* m_Canvas;

protected:
	virtual void BeginPlay() override;

public:
	UWidgetMapPanel* GetCanvas();

	virtual void PrintErrorText(const FString& str) override;
};



