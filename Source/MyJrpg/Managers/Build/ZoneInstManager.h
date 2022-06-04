// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Actors/Field/Gather/TreeBase.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "UObject/NoExportTypes.h"
#include "ZoneInstManager.generated.h"

class UNPCPaletteDataAsset;
/**
 * 생성 삭제 모두 얘기준으로 해야한다.
 * 지금이 생성인지
 * 다시 저장하는법?
 * 위치는 저장이 되지만 그외의 상태는 어떻게 저장하지?
 */
UENUM()
enum class EActorType :uint8
{
	Npc,
	Item,
	Gather,
	Build,
	Len
};
USTRUCT()
struct FZoneActor
{
	GENERATED_BODY()


	
public:
	FZoneActor()
	{
			
	}

public:
	UPROPERTY(EditAnywhere)
	FName m_IDEntity;
	UPROPERTY(EditAnywhere)
	EActorType m_nType;
	UPROPERTY(EditAnywhere)
	FVector  m_SpawnPosition;
	UPROPERTY(EditAnywhere)	
	FRotator m_SpawnRotation;
};

UCLASS()
class MYJRPG_API UZoneInstManager : public UObject
{
	GENERATED_BODY()

protected:
	TMap<FName,TArray<FZoneActor>> m_MapBuildInsts;//위치값 밖에 저장이안되는데

	TArray<TWeakObjectPtr<AMonsterPawn>> m_Npc;

	TArray<TWeakObjectPtr<AItemActor>> m_Item;

	TArray<TWeakObjectPtr<ATreeBase>> m_Gather;

	TArray<TWeakObjectPtr<AStructureActor>> m_Build;

protected:
	AMonsterPawn* SpawnNpcActor(const FZoneActor& SpawnData);

	AItemActor* SpawnItemActor(const FZoneActor& spawn_data);

	ATreeBase* SpawnGatherActor(const FZoneActor& spawn_data);

	AStructureActor* SpawnBuildActor(const FZoneActor& spawn_data);
	
	TArray<FZoneActor> CreateBuildInst(const UNPCPaletteDataAsset* npcAssets);
	
	void InitZone(const FName& id, const FZoneDataRow& zoneData);

	void SpawnActors(const TArray<FZoneActor>& zoneInst);
	
public:
	void SpawnZone(const FName& id, const FZoneDataRow& zoneData);

	void SaveActors(const FName& id);
};

