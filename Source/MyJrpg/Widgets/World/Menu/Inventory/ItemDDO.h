// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MyJrpg/Items/AssembleInst.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "ItemDDO.generated.h"

class UItemConvertInst;
struct FItemSpec;
class UInventory;
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

	TWeakObjectPtr<UInventory> m_FromInven;

	TWeakObjectPtr<UEquipManager> m_FromEquip;

	TWeakObjectPtr<UItemConvertInst> m_FromConverter;

	TWeakObjectPtr<UAssembleInst> m_FromAssemble;

	TWeakObjectPtr<UInventory> m_FromRideCost;

	int m_nIndex;
public:
	void SetDDO(UWidgetBaseElement* ele);

	const FItemSpec& GetItem();
public:
	virtual void BeginDestroy() override;
	
	UFUNCTION()
	void OnDragCancel(UDragDropOperation* meSelf);
	UFUNCTION()
	void OnMyDrop(UDragDropOperation* meSelf);
	
	void RemoveItemFromInven();
	
	void AddItemToSlot(const FItemSpec& item_spec);

protected:
	void Clear();
};


