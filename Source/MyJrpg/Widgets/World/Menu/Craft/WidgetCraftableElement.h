// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Managers/CraftManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetCraftableElement.generated.h"

/**
 * 저를 클릭하면,제작 재료가 쭈루루루 뜹니다
 */
UCLASS()
class MYJRPG_API UWidgetCraftableElement : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClicked, const FCraftable&, int index);

	FOnClicked m_OnClicked;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ItemElement;

	const FCraftable* m_PtrCraftData;

protected:
	void OnHoldComplete(UWidgetBaseElement*);//인포 띄워주기

	void OnClicked(UWidgetBaseElement*);//제작재료 생성시켜주기

public:
	void SetCraftable(const FCraftable& id, int index);

	const FCraftable& GetCraftItem() const;
};
