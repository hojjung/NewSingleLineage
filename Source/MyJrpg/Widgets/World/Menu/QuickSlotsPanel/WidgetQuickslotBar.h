// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "MyJrpg/Widgets/World/Menu/Skill/WidgetSkillPanel.h"
#include "WidgetQuickslotBar.generated.h"

/**
 * 
 */

class UWidgetQuickSlot;
class UWidgetInventory;
UCLASS()
class MYJRPG_API UWidgetQuickslotBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetBaseElement> m_ClassWidgetItemEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* m_ScrollBarQuickSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_QuickSlot;
	UPROPERTY()
	TArray<UWidgetBaseElement*> m_AryQuickSlot;

	UInventory** m_InvenHolder;
	
protected:
	void CreateBeltSlots();
	
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	void OnClick(UWidgetBaseElement* ele);

	void OnClickQuick(UWidgetBaseElement* ele);

	void SetItem(UWidgetBaseElement* target, const FItemSpec& itemSpec);

	void Clear();

	void OnBeltEquipChanged();
	
	void UpdateQuickSlots();

	void UpdateQuickOne();
};
