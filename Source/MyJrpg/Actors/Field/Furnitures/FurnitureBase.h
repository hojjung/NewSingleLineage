// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/InteractActorBase.h"
#include "MyJrpg/Interfaces/Buildable.h"
#include "MyJrpg/Widgets/World/Build/BuildWidgetCompo.h"
#include "FurnitureBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AFurnitureBase : public AInteractActorBase, public IBuildable
{
	GENERATED_BODY()

public:
	AFurnitureBase();

protected:
	UPROPERTY(VisibleAnywhere)
	UBuildWidgetCompo* m_WidgetComp;
	UPROPERTY()
	TArray<UMeshComponent*> m_AryMeshCompos;
	
	TArray<TArray<TStrongObjectPtr<UMaterialInterface>>> m_AryAryMats;
	
	const FBuildDataRow* m_BuildData;

protected:
	virtual void BeginPlay() override;
	
public:
	virtual const FBuildDataRow& GetBuildData() const override;
	virtual void SetBuildData(const FBuildDataRow& data)override;
	virtual void SetMat(UMaterialInterface* mat)override;
	virtual void ShowBuildWidget(bool b)override;
	virtual void ConfirmBuild()override;
	virtual void ShowSelect(bool b)override;
	virtual bool IsEraseable() override;
	virtual void SetColl(bool b) override;
};
