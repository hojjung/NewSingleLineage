// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "UObject/NoExportTypes.h"
#include "PlayerAnimInst.generated.h"

/**
 * 무기 장착에 따라서 기본 애니매이션 변경
 */
UCLASS()
class MYJRPG_API UPlayerAnimInst : public UObject
{
	GENERATED_BODY()

protected:
	const FPlayerUnitEntityRow* m_EntityRow;
	
	TWeakObjectPtr<AMyPlayerPawn> m_Owner;
	UPROPERTY()
	TArray<UAnimMontage*> m_AryAnims;
	UPROPERTY()
	UAnimMontage* m_BaseAttack;

	TSharedPtr<FStreamableHandle> m_BaseAttackHandle;
	
	TMap<FName,UAnimMontage*> m_MapSkills;
	
	TArray<TSharedPtr<FStreamableHandle>> m_AryHandle;

protected:
	void OnEquipChanged();

	void SetStanceSword();

	void SetStanceMage();
	
public:
	void Init(const FPlayerUnitEntityRow& row, AMyPlayerPawn* pl);

	UAnimMontage* GetSkillAnim(FName skillID);

	UAnimMontage* GetBasAttackAnim();

	void KillAll();
};

