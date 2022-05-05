// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "WidgetBaseElement.generated.h"

struct FColorDataHandle;

UCLASS()
class MYJRPG_API UWidgetBaseElement : public UUserWidget
{
	GENERATED_BODY()

 public:
 	DECLARE_MULTICAST_DELEGATE(FOnClicked);

 	FOnClicked m_OnClick;

	FOnClicked m_OnHold;

 protected:
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
 	UImage* m_ImgGlow;
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
 	UImage* m_ImgItemIcon;
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
 	UOverlay* m_OverlayFocus;
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
 	UProgressBar* m_HoldingBar;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_DefaultGlow;
	UPROPERTY()
 	float m_fTimer;
	UPROPERTY()
 	bool m_bHolding;
	UPROPERTY()
	bool m_bIsHoldable;
	UPROPERTY()
	bool m_bIsFocusable;

 protected:
 	virtual void NativeOnInitialized() override;
 	
 	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void OnHoldingComplete();
	
 	void EndHolding();
	
 public:
	UDragDropOperation* CreateDDO();

	UImage* GetImgIcon();
	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
 	void SetIcon(TSoftObjectPtr<UTexture2D> t);

	void SetGlowColor(const FColorDataHandle& color);

	bool IsFocused();
	
 	void Clear();

	void SetHoldable(bool isActive);

	void SetFocusable(bool isActive);
	
	bool IsMyFocused() const;

	void OnClick();

	void SetMyFocus();

	void SetMyUnFocus();
};
