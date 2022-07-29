// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetEventStageConfirm.h"
#include "WidgetMapBtn.h"
#include "WidgetMapPlayerIcon.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "MyJrpg/Managers/EventStageManager.h"
#include "MyJrpg/Widgets/World/Menu/CharacterInfoHUD/AlertInfoWindow.h"
#include "MyJrpg/Widgets/World/Menu/Craft/WidgetCraftPanel.h"
#include "MyJrpg/Widgets/World/Menu/Equipment/WidgetEquipInvenPanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetItemInfo.h"
#include "MyJrpg/Widgets/World/Menu/ZoneMove/WidgetZoneSelectButton.h"
#include "WidgetMapPanel.generated.h"

/**
 * UWidgetEventStageConfirm
 * 
 */
UCLASS()
class MYJRPG_API UWidgetMapPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_ParentCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UAlertInfoWindow* m_AlertInfoWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetZoneSelectButton* m_ZoneSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemInfo* m_ItemInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnInven;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCraft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipInvenPanel* m_InvenEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftPanel* m_CraftPanel;
	TWeakObjectPtr<UCanvasPanelSlot> m_PanelSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_MoveBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetMapPlayerIcon* m_PlayerIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRunCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRunTimeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetEventStageConfirm> m_ClassEventConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetMapBtn> m_ClassEventMapBtn;
	
public:
	virtual void NativeOnInitialized() override;
	
	void OpenItemInfoData(const FItemDataRow& item_data_row);
	
	void PrintInfoText(const FString& string, FLinearColor color);

protected:
	void OnClick(const FName& zoneID);

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	void OnWalkStart(const FName& dst, float dist);

	void OnWalkTick();

	void InitCreateEventBtns();

	void CreateEventBtn(const FEventStageSpec& data);
	
	void ShowEventConfirm(const FEventStageSpec& data);

	void RemoveEventBtn(const FEventStageSpec& data);

public:
	UFUNCTION()
	void OnOpenInven();
	UFUNCTION()
	void OnOpenCraft();
	UFUNCTION()
	void OnRunStart();

	void SetMoveBar(const FName& dst, float dist);

	void OnMove();

	void OnMoveEnd();
	
	void SetMapCanvasPos(FVector2D ResultPos, bool useAnim = false);
};
