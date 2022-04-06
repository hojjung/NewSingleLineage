#include "SpawnManager.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "RewardManager.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/FieldPropData.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Pawns/Components/MyMovement.h"


void USpawnManager::SetSpawnActors(const UNPCPaletteDataAsset* npcAssets)
{
	if (!npcAssets || npcAssets->m_ArySpawnDatas.Num() < 1)
	{
		return;
	}
	
	m_AryNpcActors.Reset();

	for (const FNPCSpawnData& SpawnDataEle : npcAssets->m_ArySpawnDatas)
	{
		SpawnNpcActor(SpawnDataEle, npcAssets);
	}
}

void USpawnManager::SpawnPlayer(const FVector& loc, const FRotator& rot)
{
	auto* Pl = UMyLib::GetPlayer();
	float Diff = Pl->GetCapsule()->GetScaledCapsuleHalfHeight() - 50;

	FVector LocNew = loc;

	LocNew.Z += Diff;

	Pl->SetActorLocation(LocNew, true);
	
	Pl->SetActorRotation(rot);
}

ASummonUnitPawn* USpawnManager::SummonUnit(FName unit_id, FVector loc, float lifetime)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASummonUnitPawn* NpcActor = UMyLib::GetUWorld()->SpawnActor<ASummonUnitPawn>(
		ASummonUnitPawn::StaticClass(), loc, FRotator(0), Param);

	const FNpcUnitEntityRow* EntityRow = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(unit_id, "");

	if (EntityRow)
	{
		NpcActor->SetEntity(*EntityRow);

		NpcActor->SetLifeTime(lifetime);
	}

	m_AryAllyActors.Add(NpcActor);

	return NpcActor;
}

ACombatUnitPawn* USpawnManager::GetNearNpc(FVector callerLoc, float range, TSet<ACombatUnitPawn*>* ignore)
{
	float MAX_Dist = MAX_flt;

	range = range * range;

	ACombatUnitPawn* NearPawn = nullptr;

	AMyPlayerPawn* PlayerPawn = UMyLib::GetPlayer();

	FNavLocation StartPoint;

	UNavigationSystemV1* NavSys = UMyLib::GetNavSys();

	NavSys->ProjectPointToNavigation(callerLoc, StartPoint);

	for (ACombatUnitPawn* Pawn : m_AryNpcActors)
	{
		if (!Pawn || !Pawn->IsAlive() || Pawn->IsHidden() || (ignore && (*ignore).Contains(Pawn)))
		{
			continue;
		}

		float Length = MAX_flt;

		FNavLocation EndPoint;

		NavSys->ProjectPointToNavigation(Pawn->GetActorLocation(), EndPoint);

		if (!PlayerPawn->LineOfSightTo(Pawn))
		{
			UMyLib::GetNavSys()->GetPathLength(GetWorld(), StartPoint, EndPoint, Length);

			Length = Length * Length;
		}
		else
		{
			Length = FVector::DistSquared2D(StartPoint, EndPoint);
		}

		if (range > 0 && range < Length)
		{
			continue;
		}

		if (MAX_Dist > Length)
		{
			NearPawn = Pawn;

			MAX_Dist = Length;
		}
	}

	return NearPawn;
}

AMonsterPawn* USpawnManager::SpawnNpcActor(const FNPCSpawnData& SpawnData, const UNPCPaletteDataAsset* dataAsset)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMonsterPawn* NpcActor = UMyLib::GetUWorld()->SpawnActor<AMonsterPawn>(
		AMonsterPawn::StaticClass(), SpawnData.m_SpawnPosition, SpawnData.m_SpawnRotation, Param);

	const FNpcUnitEntityRow* EntityRow = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(
		SpawnData.m_IDEntity, "");

	if (EntityRow)
	{
		NpcActor->SetEntity(*EntityRow);

		NpcActor->SetReviveTime(EntityRow->m_fMinSpawnTime, EntityRow->m_fMaxSpawnTime);

		if(UMyGameInstance::Get->m_GameRule)
		{
			UMyGameInstance::Get->m_GameRule->OnMonsterCreate(NpcActor);
		}
	}

	m_AryNpcActors.Add(NpcActor);

	return NpcActor;
}