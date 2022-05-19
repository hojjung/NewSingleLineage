// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "MyJrpg/Items/ItemConvertInst.h"
#include "BI_ItemConverter.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBI_ItemConverter : public UBuildInteractBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UItemConvertInst* m_ItemConvert;
		
public:
	virtual void Init() override;
	
	virtual bool IsEraseable() override;

	virtual void OnInteract() override;
};
