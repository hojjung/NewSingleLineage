// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"
#include "WidgetCollecItemEle.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCollecItemEle : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	
public:
	void Init(const FName& collecID,bool is_equip, const FItemDataHandle& item, int lv);
	
	void Update();
};
