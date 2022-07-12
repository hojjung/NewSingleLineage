// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "WidgetMapBtn.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetMapBtn : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClick, const FName&);

	FOnClick m_OnClick;
	
protected:
	UPROPERTY(EditAnywhere)
	FName m_ZoneID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTimeRemain;
protected:
	virtual void NativeOnInitialized() override;

	void UpdateIcon();
	
public:
	UFUNCTION()
	void OnClick();

	FName GetZoneID();
	void UpdateDuration(float dur);

	void SetZoneID(FName id, float dur);

	FVector2D GetPos();
};


