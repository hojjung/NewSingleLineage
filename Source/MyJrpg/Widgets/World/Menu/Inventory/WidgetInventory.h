// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetItemElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"

#include "WidgetInventory.generated.h"

class UDDO_InvenItem;
class UInventory;


UCLASS()
class MYJRPG_API UWidgetInventory : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetItemElement> m_ClassWidgetItemEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_InvenBox;

	UPROPERTY()
	TArray<UWidgetItemElement*> m_AryItemEles;

	TWeakObjectPtr<UWidgetItemElement> m_CurrentFocused;
	
	TWeakObjectPtr<UInventory> m_CurrentInven;

	EItemType m_FilterCategoryItem;

	bool m_bNoFilter;

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//prevent penetrate touch
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void CreateGridElements(EPanelType panelType);

	void UpdateInventory();

	bool IsSameType(const FItemSpec& item);
	
	void OnFocused(UWidgetItemElement* ele);
	
	void SetInventory(UInventory* inven,EPanelType panelType);
	
public:
	void Init(UInventory* inven,EPanelType panelType = EPanelType::Inven);

	UInventory* GetInven() const;

	const FItemSpec* GetFocusedItem() const;

	void ClosePanel();

	void UnFocusCurrent();

public://Filter
	void SetItemFilter(EItemType typeWant);

public://OnClick
	UFUNCTION()
	void ClearFilter();
	UFUNCTION()
	void OnFilterMisc();
	UFUNCTION()
	void OnFilterConsumable();
	UFUNCTION()
	void OnFilterEquips();

};



