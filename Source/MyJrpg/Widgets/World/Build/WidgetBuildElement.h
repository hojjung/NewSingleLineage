// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/BuildData.h"
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
	const FBuildDataRow* m_DataRow;

public:
	void Init(const FBuildDataRow& data);
	
	void MyFocus();
	
	void MyUnFocus();

protected:
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
