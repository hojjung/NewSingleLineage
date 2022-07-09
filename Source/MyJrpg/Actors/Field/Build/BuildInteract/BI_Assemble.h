// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "MyJrpg/Items/AssembleInst.h"
#include "MyJrpg/Items/ItemConvertInst.h"
#include "BI_Assemble.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBI_Assemble : public UBuildInteractBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UAssembleInst* m_ItemHolder;
		
public:
	virtual void Init(const TArray<FString>& variable, UInventory* inven) override;
	
	virtual bool IsEraseable() override;

	virtual void OnInteract() override;

	virtual UInventory* GetItemHolder() override;
};
