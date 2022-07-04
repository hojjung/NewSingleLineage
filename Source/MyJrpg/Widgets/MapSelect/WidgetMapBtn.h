// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
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
	UPROPERTY(EditAnywhere)
	UTexture2D* m_TextureIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnIcon;
	
protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void OnClick();

	FName GetZoneID();

	FVector2D GetPos();
};


