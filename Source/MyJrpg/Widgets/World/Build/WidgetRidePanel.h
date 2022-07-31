#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/MapSelect/WidgetRideCost.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetRidePanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetRidePanel  : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRideName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetRideCost* m_RiderCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_CostItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Inven;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Bag;

	TWeakObjectPtr<UInventory> m_RideInven;

	FDelegateHandle m_Dele;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdatePanel();

	void UpdateElement(UWidgetBaseElement* ele, const FItemSpec& item);

	void OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index);

	virtual void ClosePanel() override;

	void OnDrop(UWidgetBaseElement* ele);

	void OnDrag(UWidgetBaseElement* ele);

public:
	void ShowRidePanel(UInventory* inst);
};
