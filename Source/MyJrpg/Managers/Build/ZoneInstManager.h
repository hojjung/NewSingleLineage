// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/MinimapCam.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Actors/Field/Gather/TreeBase.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Managers/MinimapManager.h"
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
struct FZoneActorTransform
{
	GENERATED_BODY()
public:
	FZoneActorTransform(){}
	
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

USTRUCT()
struct FZoneSerialData
{
	GENERATED_BODY()

public:
	TArray<FZoneActorTransform> m_AryZoneActorTrans;

	TMap<int,int> m_MapGatherHp;

	TMap<int,TStrongObjectPtr<UInventory>> m_MapItemHolders;
};

UCLASS()
class MYJRPG_API UZoneInstManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnActorVisible, AActor*, bool);

	FOnActorVisible m_OnActorVisible;
	
protected:
	UPROPERTY()
	UMinimapManager* m_MinimapManager;
	UPROPERTY()
	TArray<TScriptInterface<IFocusable>> m_AryFocusActors;

protected:
	TWeakObjectPtr<AMinimapCam> m_MiniMapCam;
	
	TMap<FName,FZoneSerialData> m_MapBuildInsts;//위치값 밖에 저장이안되는데

	FName m_PlayerTombZoneID;

	TArray<TWeakObjectPtr<AMonsterPawn>> m_Npc;

	TArray<TWeakObjectPtr<AItemActor>> m_Item;

	TArray<TWeakObjectPtr<ATreeBase>> m_Gather;

	TArray<TWeakObjectPtr<AStructureActor>> m_Build;

protected:
	AMonsterPawn* SpawnNpcActor(const FZoneActorTransform& spawnData, int index, const FZoneSerialData& serialData);

	AItemActor* SpawnItemActor(const FZoneActorTransform& spawn_data, int index, const FZoneSerialData& serialData, bool isInit);

	ATreeBase* SpawnGatherActor(const FZoneActorTransform& spawn_data, int index, const FZoneSerialData& serialData, bool isInit);

	AStructureActor* SpawnBuildActor(const FZoneActorTransform& spawn_data, int index, const FZoneSerialData& serialData);
	
	TArray<FZoneActorTransform> CreateBuildInst(const UNPCPaletteDataAsset* npcAssets);
	
	void InitZone(const FName& id, const FZoneDataRow& zoneData);

	void SpawnActors(const FZoneSerialData& zoneInst, bool isInit);

	FZoneSerialData& GetCurentZoneSerialData();

	void AddPlayerAllItem(UInventory* inven);
	
public:
	void SpawnZone(const FName& id, const FZoneDataRow& zoneData);

	int SaveActors(const FName& id);

	void AddBuildActor(AStructureActor* buildActor);

public:
	void AddFocusActor(UObject* want);
	
	void SpawnPlayer(const FVector& loc, const FRotator& rot);

	void RemoveFocusActor(UObject* want);
	
	IFocusable* GetNearProp(FVector callerLoc, float range = 0, UClass* ignoreClass = nullptr, bool excludeNotInteractable = false);

	IFocusable* GetNearTarget(FVector callerLoc, float range = 0, bool isUseAuto = false);

	ACombatUnitPawn* GetNearNpc(FVector callerLoc, float range, const TSet<ACombatUnitPawn*>* ignore = nullptr, bool excludeDead = false);

	void GetNearNpcs(const ABaseUnitPawn* caller, TArray<ACombatUnitPawn*>& outAry, float range,const TSet<ACombatUnitPawn*>* ignore = nullptr);

	void AddTrackIcon(IFocusable* icon);
	void RemovePlayerTomb();

	void SaveActorsOnPlayerDead(const FName& id);
};

