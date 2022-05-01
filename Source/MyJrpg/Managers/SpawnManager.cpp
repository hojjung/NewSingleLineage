#include "SpawnManager.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "RewardManager.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
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

	m_AryFocusActors.Reset();

	for (const FNPCSpawnData& SpawnDataEle : npcAssets->m_ArySpawnDatas)
	{
		if (SpawnDataEle.m_EntityParentTable->RowStruct->IsChildOf(FNpcUnitEntityRow::StaticStruct()))
		{
			SpawnNpcActor(SpawnDataEle);
		}
		else if(SpawnDataEle.m_EntityParentTable->RowStruct->IsChildOf(FItemDataRow::StaticStruct()))
		{
			SpawnItemActor(SpawnDataEle);
		}
		else if(SpawnDataEle.m_EntityParentTable->RowStruct->IsChildOf(FGatherDataRow::StaticStruct()))
		{
			SpawnGatherActor(SpawnDataEle);
		}
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
		NpcActor->SetEntity(unit_id,*EntityRow);

		NpcActor->SetLifeTime(lifetime);
	}

	m_AryAllyActors.Add(NpcActor);

	return NpcActor;
}

void USpawnManager::AddFocusActor(UObject* want)
{
	TScriptInterface<IFocusable> Focus;
	Focus.SetInterface(want);
	Focus.SetObject(want);
	m_AryFocusActors.Add(Focus);
}

void USpawnManager::RemoveFocusActor(UObject* want)
{
	TScriptInterface<IFocusable> Focus;
	Focus.SetInterface(want);
	Focus.SetObject(want);
	m_AryFocusActors.Remove(Focus);
}

ACombatUnitPawn* USpawnManager::GetNearNpc(FVector callerLoc, float range, const TSet<ACombatUnitPawn*>* ignore)
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
			NavSys->GetPathLength(GetWorld(), StartPoint, EndPoint, Length);

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

void USpawnManager::GetNearNpcs(const ABaseUnitPawn* caller, TArray<ACombatUnitPawn*>& outAry, float range,
                                const TSet<ACombatUnitPawn*>* ignore)
{
	FVector Loc = caller->GetActorLocation();

	range = range * range;

	FNavLocation StartPoint;

	UNavigationSystemV1* NavSys = UMyLib::GetNavSys();

	NavSys->ProjectPointToNavigation(Loc, StartPoint);

	for (ACombatUnitPawn* Pawn : m_AryNpcActors)
	{
		if (!Pawn || !Pawn->IsAlive() || Pawn->IsHidden() || (ignore && (*ignore).Contains(Pawn)))
		{
			continue;
		}

		float Length = MAX_flt;

		FNavLocation EndPoint;

		NavSys->ProjectPointToNavigation(Pawn->GetActorLocation(), EndPoint);

		if (!caller->LineOfSightTo(Pawn))
		{
			NavSys->GetPathLength(GetWorld(), StartPoint, EndPoint, Length);

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

		outAry.Add(Pawn);
	}
}

AMonsterPawn* USpawnManager::SpawnNpcActor(const FNPCSpawnData& SpawnData)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FNpcUnitEntityRow* EntityRow = SpawnData.m_EntityParentTable->FindRow<FNpcUnitEntityRow>(
		SpawnData.m_IDEntity, "");
	
	AMonsterPawn* NpcActor = UMyLib::GetUWorld()->SpawnActor<AMonsterPawn>(
		EntityRow->m_ClassActor, SpawnData.m_SpawnPosition, SpawnData.m_SpawnRotation, Param);


	if (EntityRow)
	{
		NpcActor->SetEntity(SpawnData.m_IDEntity,*EntityRow);

		NpcActor->SetReviveTime(EntityRow->m_fMinSpawnTime, EntityRow->m_fMaxSpawnTime);

		if(UMyGameInstance::Get->m_GameRule)
		{
			UMyGameInstance::Get->m_GameRule->OnMonsterCreate(NpcActor);
		}
	}

	m_AryNpcActors.Add(NpcActor);

	return NpcActor;
}

AItemActor* USpawnManager::SpawnItemActor(const FNPCSpawnData& spawn_data)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FItemDataRow* EntityRow = spawn_data.m_EntityParentTable->FindRow<FItemDataRow>(
		spawn_data.m_IDEntity, "");
	
	AItemActor* NpcActor = UMyLib::GetUWorld()->SpawnActor<AItemActor>(
		EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation, Param);


	if (EntityRow)
	{
		NpcActor->Init(spawn_data.m_IDEntity,1);
	}

	AddFocusActor(NpcActor);

	return NpcActor;
}

ATreeBase* USpawnManager::SpawnGatherActor(const FNPCSpawnData& spawn_data)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FGatherDataRow* EntityRow = spawn_data.m_EntityParentTable->FindRow<FGatherDataRow>(
		spawn_data.m_IDEntity, "");
	
	ATreeBase* NpcActor = UMyLib::GetUWorld()->SpawnActor<ATreeBase>(
		EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation + FRotator(0,FMath::RandRange(-180,180),0), Param);

	if (EntityRow)
	{
		NpcActor->SetEntity(*EntityRow,UMyLib::GetPlayer());
	}

	AddFocusActor(NpcActor);

	return NpcActor;
}

IFocusable* USpawnManager::GetNearProp(FVector callerLoc, float range)
{
	float MAX_Dist = MAX_flt;

	range = range * range;

	IFocusable* NearPawn = nullptr;

	AMyPlayerPawn* PlayerPawn = UMyLib::GetPlayer();

	FNavLocation StartPoint;

	UNavigationSystemV1* NavSys = UMyLib::GetNavSys();

	NavSys->ProjectPointToNavigation(callerLoc, StartPoint);

	for (TScriptInterface<IFocusable>& Focus : m_AryFocusActors)
	{
		if (!Focus)
		{
			continue;
		}

		AActor* FocusActor = Cast<AActor>(Focus.GetObject());

		float Length = MAX_flt;

		FNavLocation EndPoint;

		NavSys->ProjectPointToNavigation(FocusActor->GetActorLocation(), EndPoint);

		if (!PlayerPawn->LineOfSightTo(FocusActor))
		{
			NavSys->GetPathLength(GetWorld(), StartPoint, EndPoint, Length);

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
			NearPawn = Cast<IFocusable>(Focus.GetObject());

			MAX_Dist = Length;
		}
	}

	return NearPawn;
}

IFocusable* USpawnManager::GetNearTarget(FVector callerLoc, float range)
{
	ACombatUnitPawn* Pawn = GetNearNpc(callerLoc, range);
	
	IFocusable* Prop = GetNearProp(callerLoc, range);
	AActor* FocusActor = Cast<AActor>(Prop);

	if(Pawn && FocusActor)
	{
		FVector Loc1 = Pawn->GetActorLocation();
		
		FVector Loc2 = FocusActor->GetActorLocation();
		
		if(FVector::DistSquared2D(Loc1, callerLoc) <= FVector::DistSquared2D(Loc2, callerLoc))
		{
			return 	Pawn;
		}
		return Prop;
	}
	else if (Pawn)
	{
		return Pawn;		
	}
	return Prop;
}
