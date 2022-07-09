// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetCraftCostElement.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Items/AssembleInst.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "WidgetAssemble.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetAssemble : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot06;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot07;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot08;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot09;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftCostElement* m_Slot10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPutAll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnComplete;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;

	TArray<UWidgetCraftCostElement*> m_ArySlots;

	TWeakObjectPtr<UAssembleInst> m_AssembleInst;

	FDelegateHandle m_DeleHandle;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateSlots();

	int GetStack(int index);

public:
	void ShowAssemble(UAssembleInst* assemble_inst);

	void ClosePanel();

	UFUNCTION()
	void OnPutAll();
	UFUNCTION()
	void OnComplete();
};
