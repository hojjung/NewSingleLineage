// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetShopItemEle.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetShopItemEle : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClicked,UWidgetShopItemEle*);

	FOnClicked m_OnFocus;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ElementBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCost;

	FName m_ItemID;

	int m_nMaxAmount;

protected:
	virtual void NativeOnInitialized() override;

	FText GetFocusText();

	void OnHold(UWidgetBaseElement* ele);

	void OpenStackCalculator();
	
	void OnClicked();

	bool IsSlotEmpty();

	void OnBuyConfirm(int amount);

	int GetMaxAmount();

public:
	void UpdateElement(const FName& tradeData);
	
	void Clear();

	void SetMyUnFocus();
};
