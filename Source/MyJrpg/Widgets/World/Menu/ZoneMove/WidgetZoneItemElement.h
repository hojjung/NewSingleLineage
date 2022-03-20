// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/DropData.h"
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
	
public:
	void SetZone(const FDropRewardItem& data);
};
