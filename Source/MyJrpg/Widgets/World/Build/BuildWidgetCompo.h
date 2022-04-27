// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldWidgetStruct.h"
#include "Components/WidgetComponent.h"
#include "MyJrpg/DataTables/BuildData.h"
#include "BuildWidgetCompo.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBuildWidgetCompo : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UBuildWidgetCompo();

protected:
	UPROPERTY()
	TSubclassOf<UWorldWidgetStruct> m_ClassWidget;
	UPROPERTY()
	UWorldWidgetStruct* m_Widget;

	EBuildType m_Type;
	
public:
	void Init();

	void ShowBuildWidget(bool b);
	
	void ShowSelect(bool b);
};
