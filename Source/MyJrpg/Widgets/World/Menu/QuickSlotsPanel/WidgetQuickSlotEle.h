// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Items/Inventory.h"
#include "Components/ProgressBar.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetCircleIcon.h"
#include "WidgetQuickSlotEle.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuickSlotEle : public UUserWidget
{
	GENERATED_BODY()


 public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocus, UWidgetQuickSlotEle*);
	
	FOnFocus m_OnClick;

 protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_Size;
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
 	UWidgetCircleIcon* m_ImgItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextStackAmount;//Level
	
	int m_nIndex;

	const FItemDataRow* m_ItemData;

 protected:
 	virtual void NativeOnInitialized() override;
 	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	UWidget* GetImgIcon();
	
 	void Clear();
	
 	void SetIcon(TSoftObjectPtr<UTexture2D> t);

	void SetTextStackLv(FString t);

	void SetTextStackLv(FText t);

	void HideTextStackLv();

	void SetIndex(int index);

	int GetIndex();
	
	void SetBoxSize(float x, float y);

	void SetItem(const FItemSpec& itemSpec);

	void SetItemData(const FItemDataRow& itemData);

	const FItemDataRow* GetItemData() const;
};


