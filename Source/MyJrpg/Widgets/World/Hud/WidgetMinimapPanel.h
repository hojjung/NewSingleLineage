// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMinimapIcon.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Actors/MyPlayerController.h"
#include "WidgetMinimapPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetMinimapPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetMinimapIcon> m_ClassWidgetMinimapIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_MainMapCanvas;

protected:
	TWeakObjectPtr<AMyPlayerController> m_PlCon;
	
	TSet<AActor*> m_VisibleActors;

	TMap<AActor*, UCanvasPanelSlot*> m_MapIcons;

	FVector2D m_ScreenSize;

	FVector2D m_CanvasSize;

	FVector2D m_RatioSize;
	
protected:
	void OnActorVisible(AActor* actorTarget, bool isVisible);

	virtual void NativeOnInitialized() override;
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
