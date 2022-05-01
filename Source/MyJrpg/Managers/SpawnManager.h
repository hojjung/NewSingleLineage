#pragma once

#include "CoreMinimal.h"
#include "NPCPaletteDataAsset.h"
#include "HelperClasses/GridPawnLocater.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "MyJrpg/Pawns/SummonUnitPawn.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "UObject/NoExportTypes.h"
#include "SpawnManager.generated.h"

class ATreeBase;
class AMonsterPawn;
class ACombatUnitPawn;

UCLASS()
class MYJRPG_API USpawnManager : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryNpcActors;
	UPROPERTY()
	TArray<TScriptInterface<IFocusable>> m_AryFocusActors;
	UPROPERTY()
	TArray<ASummonUnitPawn*> m_AryAllyActors;

protected:
	AMonsterPawn* SpawnNpcActor(const FNPCSpawnData& SpawnData);
	
	AItemActor* SpawnItemActor(const FNPCSpawnData& spawn_data);

	ATreeBase* SpawnGatherActor(const FNPCSpawnData& spawn_data);
	
public:
	ACombatUnitPawn* GetNearNpc(FVector callerLoc, float range = 0, const TSet<ACombatUnitPawn*>* ignore = nullptr);

	IFocusable* GetNearProp(FVector callerLoc, float range = 0);

	IFocusable* GetNearTarget(FVector callerLoc, float range = 0);

	void GetNearNpcs(const ABaseUnitPawn* caller, TArray<ACombatUnitPawn*>& outAry, float range = 0, const TSet<ACombatUnitPawn*>* ignore = nullptr);
	
	void SetSpawnActors(const UNPCPaletteDataAsset* npcAssets);
	
	void SpawnPlayer(const FVector& loc, const FRotator& rot);

	ASummonUnitPawn* SummonUnit(FName unit_id ,FVector loc,float lifetime);
	
	TArray<AMonsterPawn*>& GetAllMobs()
	{
		return m_AryNpcActors;
	}

	void AddFocusActor(UObject* want);

	void RemoveFocusActor(UObject* want);
};