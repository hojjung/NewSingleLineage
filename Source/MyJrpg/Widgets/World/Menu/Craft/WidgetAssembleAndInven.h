// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetAssemble.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetAssembleAndInven.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetAssembleAndInven : public UWidgetBasePanel
{
	GENERATED_BODY()

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetAssemble* m_AssemblePanel;
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

	virtual void OpenPanel() override;
	
public:
	UWidgetInventory* GetInvenPanel();

	UWidgetAssemble* GetAssemble();

	virtual void ClosePanel() override;

	void ShowAssemble(UAssembleInst* inst);
};
