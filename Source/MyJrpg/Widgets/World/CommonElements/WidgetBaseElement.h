// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "WidgetBaseElement.generated.h"

struct FColorDataHandle;
/**
 * 인벤토리
 * 장비칸
 * 스킬정보
 * 제작
 * 아이템 드랍 몬스터정보
 *
 * 클릭과 꾹누루기 지원필요
 *
 * 클릭과 홀드로 주는 정보가 다른데?
 *
 * 정보를 아예 따로 담아줘,자식이 담게하기
 *
 * 포커싱도 항상되야함,그게 온클릭임
 *
 * 칸을 비우는것도 가능해야함
 */
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
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
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
