// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "MyJrpg/Items/Inventory.h"
#include "BI_Storage.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBI_Storage : public UBuildInteractBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UInventory* m_Inven;
	
public:
	virtual void Init() override;
	
	virtual bool IsEraseable() override;

	virtual void OnInteract() override;
};
