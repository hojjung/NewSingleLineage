// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetCollecItemEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"
#include "WidgetCollecPanelChild.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCollecPanelChild : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextStat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* m_HoriElements;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetCollecItemEle> m_ClassItemEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextProgress;

	FName m_CollectionID;

protected:
	void SetOptionText(TSubclassOf<UOptionBase> classOp);
	
public:
	void Init(const FName& collecID ,const FItemCollecRow& row);

	void Update();
};
