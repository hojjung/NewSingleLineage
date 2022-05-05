// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDDO.generated.h"

class UWidgetBaseElement;
class UWidgetItemElement;
/**
 * 
 */
UCLASS()
class MYJRPG_API UItemDDO : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	static UItemDDO* GetDDOInst;
	
public:
	void SetDDO(UWidgetBaseElement* ele);

public:
	UFUNCTION()
	void OnDragCancel(UDragDropOperation* meSelf);
	UFUNCTION()
	void OnDrop(UDragDropOperation* meSelf);
};
