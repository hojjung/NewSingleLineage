// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "UObject/NoExportTypes.h"
#include "PlSkillAuto.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UPlSkillAuto : public UObject
{
	GENERATED_BODY()

protected:
	TQueue<USkillBase*> m_QueSkillInstsWait;

	TSet<USkillBase*> m_SetSkillInstsWait;

	bool m_bAutoUse;

	UPROPERTY()
	AMyPlayerPawn* m_Pl;
	
	const TArray<USkillBase*>* m_PtrSkills;
	
protected:
	void TryAddWaitQue(USkillBase* skillInst);

	void TryUseWaitQue();

public:
	void Init();
	
	void SetUseAuto(bool bUseAuto);
	
	void Tick(float deltaTime);
};


