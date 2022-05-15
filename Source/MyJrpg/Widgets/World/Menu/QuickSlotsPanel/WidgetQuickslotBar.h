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
	TWeakObjectPtr<UWidgetInventory> m_WidgetInven;

	TWeakObjectPtr<UWidgetSkillPanel> m_WidgetSkill;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetQuickSlot> m_ClassQuickSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollBarQuickSlot;
	UPROPERTY()
	TArray< UWidgetQuickSlot*> m_AryQuickSlot;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void UpdateQuickSlots();

	void SetInvenSkill(UWidgetInventory* inven,UWidgetSkillPanel* skill);

	UWidgetInventory* GetInven() const
	{
		return m_WidgetInven.Get();
	}

	UWidgetSkillPanel* GetSkill() const
	{
		return m_WidgetSkill.Get();
	}
	void OnStartCD(int index);
};
