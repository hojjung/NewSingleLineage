// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetItemElement.h"
#include "WidgetCraftableElement.generated.h"

/**
 * 저를 클릭하면,제작 재료가 쭈루루루 뜹니다
 */
UCLASS()
class MYJRPG_API UWidgetCraftableElement : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClicked,UWidgetCraftableElement*, int);

	FOnClicked m_OnClicked;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ItemElement;

	int m_nIndex;
	
protected:
	void OnHoldComplete();//인포 띄워주기

	void OnClicked();//제작재료 생성시켜주기

public:
	void SetCraftable(int index);

	void SetUnfocus();

	void SetMyFocus();

	int GetIndex() const;
};
