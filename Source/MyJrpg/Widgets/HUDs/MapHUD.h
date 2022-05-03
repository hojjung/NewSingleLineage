// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyJrpg/Widgets/MapSelect/WidgetMapPanel.h"
#include "MapHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AMapHUD : public AHUD
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
	
};



