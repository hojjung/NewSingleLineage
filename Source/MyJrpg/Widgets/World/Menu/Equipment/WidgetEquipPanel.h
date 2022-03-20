#pragma once

#include "CoreMinimal.h"
#include "WidgetEquipElement.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetItemElement.h"
#include "WidgetEquipPanel.generated.h"

UCLASS()
class MYJRPG_API UWidgetEquipPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Weapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Head;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Torso;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Hand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Leg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_EarRing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Ring;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Neckless;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipElement* m_Belt;
	UPROPERTY()
	TArray<UWidgetEquipElement*> m_AryEquips;

	EItemType m_FocusType;
	
protected:
	void UpdateSlots();

	void RequestFilterItems();
	
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

};


