// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Buildable.generated.h"

struct FBuildDataRow;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYJRPG_API IBuildable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const FBuildDataRow& GetBuildData() const = 0 ;
	virtual void SetBuildData(const FBuildDataRow& data) = 0 ;
	virtual void SetMat(UMaterialInterface* mat) = 0 ;
	virtual void ShowBuildWidget(bool b) = 0 ;
	virtual void ConfirmBuild() = 0;
	virtual void ShowSelect(bool cond) = 0;
	virtual bool HasNextUpgrade();
	virtual bool IsEraseable();
};
