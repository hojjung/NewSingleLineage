// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "WidgetCircleIcon.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCircleIcon : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UWidgetCircleIcon(const FObjectInitializer& obj);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (ExposeOnSpawn))
	UTexture2D* m_IconWant;
	UPROPERTY()
	UMaterialInterface* m_MatTemplate;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* m_MatInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImageIcon;
	
protected:
	virtual void NativePreConstruct() override;

public:
	void SetIcon(UTexture2D* t);
};
