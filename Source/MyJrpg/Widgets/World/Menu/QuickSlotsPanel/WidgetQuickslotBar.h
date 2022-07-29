// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "WidgetQuickSlotEle.h"
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
	TWeakObjectPtr<UInventory> m_InvenHolder;
	UPROPERTY()
	TArray<UWidgetQuickSlotEle*> m_AryQuickSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuickSlotEle* m_Quick01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuickSlotEle* m_Quick02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuickSlotEle* m_Quick03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuickSlotEle* m_Quick04;
	
protected:
	void SetQuickSlot();
	virtual void NativeOnInitialized() override;
	
	void OnInvenChanged();

	void OnClick(UWidgetQuickSlotEle* ele);

	void SetItem(UWidgetQuickSlotEle* target, const FItemSpec& itemSpec);
};
