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
	TArray<ASummonUnitPawn*> m_AryAllyActors;

protected:
	AMonsterPawn* SpawnNpcActor(const FNPCSpawnData& SpawnData,const UNPCPaletteDataAsset* dataAsset);
public:
	ACombatUnitPawn* GetNearNpc(FVector callerLoc, float range = 0, TSet<ACombatUnitPawn*>* ignore = nullptr);

public:
	void SetSpawnActors(const UNPCPaletteDataAsset* npcAssets);
	
	void SpawnPlayer(const FVector& loc, const FRotator& rot);

	ASummonUnitPawn* SummonUnit(FName unit_id ,FVector loc,float lifetime);
	
	TArray<AMonsterPawn*>& GetAllMobs()
	{
		return m_AryNpcActors;
	}
	//
	const TArray<AMonsterPawn*>& GetGridMobs(const AMonsterPawn* from_me);
};

