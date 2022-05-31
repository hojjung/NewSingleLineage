// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMapBtn.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetItemInfo.h"
#include "MyJrpg/Widgets/World/Menu/ZoneMove/WidgetZoneSelectButton.h"
#include "WidgetMapPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetMapPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetZoneSelectButton* m_ZoneSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemInfo* m_ItemInfo;
	TWeakObjectPtr<UCanvasPanelSlot> m_PanelSlot;
public:
	virtual void NativeOnInitialized() override;
	
	void OpenItemInfoData(const FItemDataRow& item_data_row);

protected:
	void OnClick(const FName& zoneID);

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};
