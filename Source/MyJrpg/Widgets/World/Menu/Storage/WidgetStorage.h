// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetStorage.generated.h"

UCLASS()
class MYJRPG_API UWidgetStorage : public UWidgetBasePanel
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_StoragePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_InvenPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Bag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Belt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWithdrawAll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnDepositAll;

protected:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FAddItem, FItemSpec&,bool);

	void AddRemoveItem(UInventory* from, FAddItem to, int index);
	
	void AddRemoveItemAll(UInventory* from, FAddItem to);
	
protected:
	virtual void NativeOnInitialized() override;

	void OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index);

	void OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index);
	
	void OnStorageInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index);
	
	void OnStorageFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index);
	
public:
	void SetTargetInven(UInventory* storage);

	virtual void ClosePanel() override;
	UFUNCTION()
	void OnWithdrawAll();
	UFUNCTION()
	void OnDepositAll();
};
