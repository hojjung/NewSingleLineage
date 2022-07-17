// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetZoneItemElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetZoneItemElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ElementBase;

protected:
	const FItemDataRow* m_ItemRow;

protected:
	void OnHold(UWidgetBaseElement* ele);
	
public:
	void SetZone(const FItemDataRow& itemID);
};
