// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Items/ItemConvertInst.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetItemConvert.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetItemConvert : public UWidgetBasePanel
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_LeftItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_FuelItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_RightItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_CostItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextConverter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Inven;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Bag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Belt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarArrow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRemainTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarFireTime;

	FDelegateHandle m_Dele;

	TArray<UWidgetBaseElement*> m_AryItems;
	
	TWeakObjectPtr<UItemConvertInst> m_ItemConvertInst;
	
protected:
	virtual void NativeOnInitialized() override;

	void SetProgressBar(float v);
	
	void SetFireBar(float v);

	virtual void ClosePanel() override;

	void UpdatePanel();

	void UpdateElement(UWidgetBaseElement* ele, const FItemSpec& item);

	void OnDrag(UWidgetBaseElement* ele);

	void OnDrop(UWidgetBaseElement* ele);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void ShowItemConvert(UItemConvertInst* inst);
};
