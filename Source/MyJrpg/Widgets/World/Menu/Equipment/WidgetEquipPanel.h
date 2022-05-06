#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetEquipPanel.generated.h"

class UEquipManager;
UCLASS()
class MYJRPG_API UWidgetEquipPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Weapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Head;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Torso;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Hand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Leg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_EarRing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Ring;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Neckless;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Belt;
	UPROPERTY()
	TArray<UWidgetBaseElement*> m_AryEquips;

	EItemType m_FocusType;

	FDelegateHandle m_Handle;

	TWeakObjectPtr<UWidgetBaseElement> m_CurrentFocused;

	TWeakObjectPtr<UEquipManager> m_Equip;

	TWeakObjectPtr<UInventory> m_Inven;
	
protected:
	void UnFocusCurrent();
	
	void UpdateSlots();

	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	void UpdateElement(UWidgetBaseElement* ele,  const FItemSpec& item);

	void OnFocused(UWidgetBaseElement* ele);

	void OnDrag(UWidgetBaseElement* ele);

	void OnDrop(UWidgetBaseElement* ele);

public:
	void Open();

	void Close();
};


