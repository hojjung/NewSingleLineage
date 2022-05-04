// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetItemElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"

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

	FDelegateHandle m_InvenDele;

	FDelegateHandle m_EquipDele;

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//prevent penetrate touch
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void CreateGridElements(EPanelType panelType);

	void UpdateInventory();

	bool IsFilterType(const FName& item);
	
	void OnFocused(UWidgetItemElement* ele);
	
public:
	void Init(UInventory* inven,EPanelType panelType = EPanelType::Inven);

	void OpenPanel();
	
	void ClosePanel();

	UInventory* GetInven() const;


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



