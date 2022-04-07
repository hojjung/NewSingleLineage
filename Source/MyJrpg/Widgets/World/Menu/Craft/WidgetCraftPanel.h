#pragma once

#include "CoreMinimal.h"
#include "ImageText.h"
#include "WidgetCraftFilterBtns.h"
#include "Components/ScrollBox.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "WidgetCraftPanel.generated.h"

class UWidgetFilterBtns;
struct FItemDataRow;
class UWidgetCraftCostElement;
class UWidgetCraftableElement;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCraftPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

public:
	UWidgetCraftPanel(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY()
	UTexture2D* m_CoinIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetCraftableElement> m_ClassCraftableElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetCraftCostElement> m_ClassCostElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollFilterBtns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollCraftables;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollCosts;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_GoldIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGoldCostAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_CurrentCraftItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCurrentItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* m_CountBtns;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeftCnt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRightCnt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCraft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOpenCalculator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetFilterBtns* m_ItemFilter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftFilterBtns* m_EquipFilter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftColorFilterBtns* m_ColorFilter;
	
protected:
	UPROPERTY()
	TArray<UWidgetCraftCostElement*> m_AryEle;
	
	TWeakObjectPtr<UWidgetCraftableElement> m_SelectedElement;
	
	EItemType m_FilterCategoryItem;

	EEquipSlotType m_FilterCategoryEquip;

	FName m_FilterColorData;

	int m_nCnt;

protected:
	virtual void NativeOnInitialized() override;
	
	void CreateAllCraftWidget(const TMap<FName, const FCraftItemCost*>& mapItems);

	void OnSelectCraftItem(UWidgetCraftableElement* selectedElement, FName id);

	void CreateCostWidgets(const FItemDataRow& costData);

	void UpdateCraftablePanel();
	
	void UpdateCraftCostPanel();

	void SetAmount(int amount);

	int GetMaxAmount();

	bool IsFilterType(const FItemDataRow& itemData);
	
public:
	virtual void ClosePanel() override;
	
	UFUNCTION()
	void Craft();
	UFUNCTION()
	void OpenCalculator();
	UFUNCTION()
	void OnLeftCnt();
	UFUNCTION()
	void OnRightCnt();
	UFUNCTION()
	void ClearFilter();
	UFUNCTION()
	void OnFilterItem();
	UFUNCTION()
	void OnFilterEquip();
	UFUNCTION()
	void OnFilterColor();
};
