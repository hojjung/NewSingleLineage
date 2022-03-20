// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlertInfoWindow.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "WidgetItemObtainPanel.generated.h"

/**
 * 
 */
USTRUCT()
struct FItemObtainQue
{
	GENERATED_BODY()

public:
	FItemObtainQue(const FItemDataRow* data, int amount): m_Data(data), m_nAmount(amount)
	{
	
	}
	FItemObtainQue(): m_Data(nullptr), m_nAmount(0)
	{
	
	}

public:
	const FItemDataRow* m_Data;

	int m_nAmount;

};

UCLASS()
class MYJRPG_API UWidgetItemObtainPanel : public UAlertInfoWindow
{
	GENERATED_BODY()

protected:
	TQueue<FItemObtainQue> m_QueObtained;

	int m_nIter;
	
protected:
	virtual void NativeOnInitialized() override;

	void OnItemObtain(const FItemDataRow& item, int amount);

	void OnItemObtainQue();
};
