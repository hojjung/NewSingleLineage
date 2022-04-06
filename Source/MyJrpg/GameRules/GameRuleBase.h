// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "UObject/NoExportTypes.h"
#include "GameRuleBase.generated.h"

/**
 * 몬스터 스폰 리스폰도 제어할수 있어야하지 않나
 */
UCLASS(Abstract)
class MYJRPG_API UGameRuleBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnMonsterCreate(AMonsterPawn* monster_pawn) {};
	
	virtual void OnMonsterDead(AMonsterPawn* monster_pawn) {}
};
