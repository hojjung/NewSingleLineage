// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Items/Inventory.h"
#include "WidgetBaseElement.generated.h"

struct FColorDataHandle;

UCLASS()
class MYJRPG_API UWidgetBaseElement : public UUserWidget
{
	GENERATED_BODY()

 public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocus, UWidgetBaseElement*);
	
	FOnFocus m_OnHold;

	FOnFocus m_OnFocus;

	FOnFocus m_OnDrag;

	FOnFocus m_OnDrop;

 protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_Size;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextStackAmount;//Level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_Dur;//	
 	float m_fTimer;
	
 	bool m_bHolding;
	
	bool m_bIsHoldable;
	
	bool m_bIsFocusable;

	bool m_bIsDragable;

	int m_nIndex;

 protected:
 	virtual void NativeOnInitialized() override;
 	
 	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
 	void EndHolding();
	
	UDragDropOperation* CreateDDO();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void ShowDurBar(float per);
	
	void HideDurBar();
	
	UWidget* GetImgIcon();
	
 	void Clear();
	
 	void SetIcon(TSoftObjectPtr<UTexture2D> t);

	void SetGlowColor(const FColorDataHandle& color);

	void SetHoldable(bool isActive);

	void SetFocusable(bool isActive);

	void SetDragable(bool isActive);
	
	bool IsMyFocused() const;

	void SetMyFocus();

	void SetMyUnFocus();
	
	void SetTextStackLv(FString t);

	void SetTextStackLv(FText t);

	void HideTextStackLv();

	void SetIndex(int index);

	int GetIndex();
	
	void SetTextFocus(FText t);

	void SetBoxSize(float x, float y);

	void SetItem(const FItemSpec& itemSpec);

	void SetItemData(const FItemDataRow& itemData);
};
