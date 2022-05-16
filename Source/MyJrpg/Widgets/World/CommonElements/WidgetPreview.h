// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MyJrpg/Interfaces/PreviewProxy.h"
#include "WidgetPreview.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPreview : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_PreviewImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	UMaterialInterface* m_PreviewMat;

	TScriptInterface<IPreviewProxy> m_PreviewProxy;
	
protected:
	virtual void NativePreConstruct() override;

	virtual void NativeDestruct() override;
public:
	void Init(UObject* proxyObj);
	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};

