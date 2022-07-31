// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetConfirmPanel.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void PrintInfoText(const FString& str, FLinearColor color = FLinearColor::White);

	virtual void OpenItemInfo(const FItemDataRow& item_data);

public:
	virtual void SetConfirmPanel(const FText& txt,UWidgetConfirmPanel::FOnClick onCancel, UWidgetConfirmPanel::FOnClick onConfirm);
};



