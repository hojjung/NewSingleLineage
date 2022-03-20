// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyJrpg/MyJrpg.h"
#include "WidgetFilterBtns.generated.h"

class UWidgetCraftPanel;
class UWidgetShopPanel;
class UWidgetInventory;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetFilterBtns : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClearFilter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnFilterMisc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnFilterConsume;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnFilterEquip;
	UPROPERTY()
	UButton* m_BtnSelected;
	
	EItemType m_ItemType;
	
protected:
	virtual void NativeOnInitialized() override;
	
	void OnBtnPressed(UButton* btn , EItemType t);
	
public:
	void RegisterFilter(UWidgetInventory* inven);

	void RegisterFilter(UWidgetShopPanel* shop);

	void RegisterFilter(UWidgetCraftPanel* craft);
	
	EItemType GetCrntItemType();
	
	void ClearFilter();

public:
	UFUNCTION()
	void OnFilterClear();
	UFUNCTION()
	void OnFilterMisc();
	UFUNCTION()
	void OnFilterConsume();
	UFUNCTION()
	void OnFilterEquip();
};
