// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Items/Inventory.h"
#include "UObject/NoExportTypes.h"
#include "BuildInteractBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBuildInteractBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(const FString& variable, UInventory* inven) {}
	
	virtual UInventory* GetItemHolder() {return nullptr ;}
	
	virtual bool IsEraseable() { return true;}

	virtual void OnInteract() {}
};
