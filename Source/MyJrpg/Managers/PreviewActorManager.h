// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Interfaces/PreviewProxy.h"
#include "MyJrpg/Pawns/ModularUnitPawn.h"
#include "UObject/NoExportTypes.h"
#include "PreviewActorManager.generated.h"

class APreviewActor;
/**
 * 모듈러 메쉬 한개로 통합? 
 */
UCLASS()
class MYJRPG_API UPreviewActorManager : public UObject, public IPreviewProxy
{
	GENERATED_BODY()

protected:

	
	TWeakObjectPtr<APreviewActor> m_PreviewActor;

public:
	void CreatePreviewActor();

	virtual void SetIsTouched(bool b) override;

	virtual void RotatePawn(float delta_x) override;

	void ShowPawn();

	void HidePawn();
	
	void Update(const AModularUnitPawn* pawn);
};
