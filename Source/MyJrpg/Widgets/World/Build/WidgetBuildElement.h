// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/BuildData.h"
#include "MyJrpg/Widgets/World/Menu/Craft/WidgetCraftCostElement.h"
#include "WidgetBuildElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetBuildElement : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClick, UWidgetBuildElement*,const FBuildDataRow&);

	FOnClick m_OnClick;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCnt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_VertCost;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidgetCraftCostElement> m_ClassCostElement;
	UPROPERTY()
	TArray<UWidgetCraftCostElement*> m_CraftCost;
	
protected:
	const FBuildDataRow* m_DataRow;

public:
	void Init(const FBuildDataRow& data);

	void SetStackCount(int cnt);

	void MyFocus();
	
	void MyUnFocus();

	void CreateCostWidgets(const TArray<FCraftItemCost>& costData);
	
	void UpdateCost();
protected:
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


};
