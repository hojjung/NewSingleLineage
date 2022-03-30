// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyJrpg/DataTables/ShopData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "UWidgetShopItemElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UUWidgetShopItemElement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClicked,UUWidgetShopItemElement*);

	FOnClicked m_OnFocus;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ElementBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCost;

	const FItemTradingData* m_TradeData;

	int m_nMaxAmount;

protected:
	virtual void NativeOnInitialized() override;

	FText GetFocusText();

	void OnHoldingComplete();

	void OpenStackCalculator();
	
	void OnClicked();

	bool IsSlotEmpty();

	void OnBuyConfirm(int amount);

	int GetMaxAmount();

public:
	void UpdateElement(const FItemTradingData& tradeData);
	
	void Clear();

	void SetMyUnFocus();
};
