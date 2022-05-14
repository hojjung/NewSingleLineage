// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetEquipPanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"

#include "WidgetEquipInvenPanel.generated.h"

class UWidgetStatPanel;
class UWidgetInventory;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetEquipInvenPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetStatPanel* m_StatPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipPanel* m_EquipPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_InvenPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_BagPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_BeltPanel;

protected:
	FDelegateHandle m_DeleBag;

	FDelegateHandle m_DeleBelt;
	
protected:
	virtual void NativeOnInitialized() override;

	void OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index);

	void OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index);

	void OnEquipChanged();

	void OnBagBeltChanged();

public:
	UWidgetInventory* GetInvenPanel();

	UWidgetEquipPanel* GetEquipPanel();

	UWidgetStatPanel* GetStatPanel();

	virtual void ClosePanel() override;
	
	void OpenInventory();
};



