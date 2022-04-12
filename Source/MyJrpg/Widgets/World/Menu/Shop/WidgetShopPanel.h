// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UWidgetShopItemElement.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/Storage/WidgetStorage.h"
#include "WidgetShopPanel.generated.h"

class UWidgetItemElement;
class UWidgetFilterBtns;
class UWidgetInventory;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetShopPanel : public UWidgetBasePanel
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUWidgetShopItemElement> m_ClassWidgetItemEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_InvenBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetFilterBtns* m_FilterBtns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_InvenPanel;
protected:
	UPROPERTY()
	TArray<UUWidgetShopItemElement*> m_AryItemEles;
	
	const TArray<FName>* m_AryItemKeys;

	TWeakObjectPtr<UUWidgetShopItemElement> m_CurrentFocused;
	
	TWeakObjectPtr<UInventory> m_CurrentInven;

	EItemType m_FilterCategoryItem;

	bool m_bNoFilter;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void UnFocusCurrent();
	
	void OnFocus(UUWidgetShopItemElement* ele);

	bool IsSameType(const FName& itemID);
	
	void UpdateShopPanel();
	
	void SetItemFilter(EItemType typeWant);

	virtual void ClosePanel() override;

public:
	UFUNCTION()
	void ClearFilter();
	UFUNCTION()
	void OnFilterMisc();
	UFUNCTION()
	void OnFilterConsumable();
	UFUNCTION()
	void OnFilterEquips();

	void SetShopPanel(const FName& shopTable);
};
