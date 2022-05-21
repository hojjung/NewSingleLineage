// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetReceiptEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "MyJrpg/DataTables/ItemConvertTable.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "WidgetReceiptPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetReceiptPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidgetReceiptEle> m_ClassReceiptEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_Scroll;
	
public:
	void SetReceipt(const FItemConvertRow& item_convert_row);
	
	virtual void ClosePanel() override;

	virtual void OpenPanel() override;
};
