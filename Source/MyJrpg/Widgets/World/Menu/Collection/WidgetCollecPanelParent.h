// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetCollecPanelChild.h"
#include "WidgetCollecStatParent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetItemInfo.h"
#include "WidgetCollecPanelParent.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCollecPanelParent : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCollecStatParent* m_TotalStat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_Scroll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetCollecPanelChild> m_ClassChild;
	
	FDelegateHandle m_UpdateElements;
	
	TArray<FDelegateHandle> m_AryUpdateElements;

protected:
	virtual void NativeOnInitialized() override;

	void CreateElements();

	void UpdateElements();


public:
	void Open();
	UFUNCTION()
	void OnClose();
};
