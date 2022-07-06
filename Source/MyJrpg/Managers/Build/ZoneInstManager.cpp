#include "ZoneInstManager.h"
#include "NPCPaletteDataAsset.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/DataTables/BuildData.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UZoneInstManager::InitZone(const FName& id, const FZoneDataRow& zoneData)
{
	FZoneSerialData NewSerialData;
	
	NewSerialData.m_AryZoneActorTrans = CreateBuildInst(zoneData.m_SpawnDataNpc);
	
	FZoneSerialData& BuildAdded = m_MapBuildInsts.Emplace(id, NewSerialData);
	
	SpawnActors(BuildAdded, true);
}

void UZoneInstManager::StartEventSpawn(bool isInit, const FZoneDataRow& zoneData, const FZoneSerialData& data)
{
	if(zoneData.m_SpawnEventID.IsNone())
	{
		return;
	}
	const FSpawnDataRow& SpawnDataRow = *USpawnEventTable::GetSpawnEventTable->FindRow<FSpawnDataRow>(zoneData.m_SpawnEventID,"");
	
	if(isInit)
	{
		UMyGameInstance::Get->m_SpawnEvent->StartSpawn(SpawnDataRow);
	}
	else
	{
		UMyGameInstance::Get->m_SpawnEvent->StartSpawn(SpawnDataRow, data.m_fSpawnEventTimer, data.m_nSpawnEventIndex);
	}
}

void UZoneInstManager::SpawnZone(const FName& id, const FZoneDataRow& zoneData)
{
	m_Npc.Reset();
	
	m_Item.Reset();
	
	m_Gather.Reset();
	
	m_Build.Reset();

	FActorSpawnParameters Param;
	Param.bNoFail = false;
	
	m_MiniMapCam = GetWorld()->SpawnActor<AMinimapCam>(Param);

	AddTrackIcon(UMyLib::GetPlayer());

	FZoneSerialData* BuildInst = m_MapBuildInsts.Find(id);
	
	if (!BuildInst)
	{
		InitZone(id, zoneData);

		StartEventSpawn(true, zoneData, *BuildInst);
		return ;
	}
	SpawnActors(*BuildInst, false);

	StartEventSpawn(false, zoneData, *BuildInst);
}

int UZoneInstManager::SaveActors(const FName& id)
{
	FZoneSerialData* BuildInst = m_MapBuildInsts.Find(id);

	*BuildInst = FZoneSerialData();
	
	UMyGameInstance::Get->m_SpawnEvent->SaveSpawnEvent(*BuildInst);

	int Index = 0;

	for(TWeakObjectPtr<AMonsterPawn> ActorEle : m_Npc)
	{
		if(!ActorEle.Get() || !ActorEle->IsAlive() && ActorEle->GetInven()->IsInvenEmpty())
		{
			continue;
		}

		FZoneActorTransform ZoneData;
		ZoneData.m_nType = EActorType::Npc;
		ZoneData.m_IDEntity = ActorEle->GetEntityID();
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->m_AryZoneActorTrans.Add(ZoneData);

		if(ActorEle->GetInven())
		{
			TStrongObjectPtr<UInventory> ItemHolder(ActorEle->GetInven());
			BuildInst->m_MapItemHolders.Add(Index, ItemHolder);

			BuildInst->m_MapGatherHp.Add(Index, ActorEle->IsAlive() ? ActorEle->GetHp() : 0);
		}
		Index++;
	}
	for(TWeakObjectPtr<ATreeBase> ActorEle : m_Gather)
	{
		if(!ActorEle.Get())
		{
			continue;
		}
		FZoneActorTransform ZoneData;
		ZoneData.m_nType = EActorType::Gather;
		ZoneData.m_IDEntity = ActorEle->GetID();
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->m_AryZoneActorTrans.Add(ZoneData);

		if(ActorEle->GetHP() > 0)
		{
			BuildInst->m_MapGatherHp.Add(Index,ActorEle->GetHP());
		}
		Index++;
	}
	for(TWeakObjectPtr<AStructureActor> ActorEle : m_Build)
	{
		if(!ActorEle.Get())
		{
			continue;
		}
		FZoneActorTransform ZoneData;
		ZoneData.m_nType = EActorType::Build;
		ZoneData.m_IDEntity = ActorEle->GetBuildData().m_RowID;
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->m_AryZoneActorTrans.Add(ZoneData);

		if(ActorEle->GetItemHolder())
		{
			TStrongObjectPtr<UInventory> ItemHolder(ActorEle->GetItemHolder());
			
			BuildInst->m_MapItemHolders.Add(Index, ItemHolder);
		}
		Index++;
	}
	for(TWeakObjectPtr<AItemActor> ActorEle : m_Item)
	{
		if(!ActorEle.Get())
		{
			continue;
		}
		FZoneActorTransform ZoneData;
		ZoneData.m_nType = EActorType::Item;
		ZoneData.m_IDEntity = ActorEle->GetItemSpec().m_ID;
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		
		BuildInst->m_AryZoneActorTrans.Add(ZoneData);
		Index++;
	}
	
	return Index;
}

void UZoneInstManager::AddBuildActor(AStructureActor* buildActor)
{
	m_Build.Add(buildActor);

	IFocusable* Focus = Cast<IFocusable>(buildActor);

	if(Focus && Focus->IsFocusable())
	{
		UMyGameInstance::Get->m_ZoneInst->AddFocusActor(buildActor);
	}
}

AMonsterPawn* UZoneInstManager::SpawnAdditionalNpcActor(const FUnitDataHandle& data)
{
	FVector PlPos = UMyLib::GetPlayer()->GetActorLocation();

	FNavLocation ResultPos;
	
	UMyLib::GetNavSys()->GetRandomPointInNavigableRadius(PlPos, 2400.f, ResultPos);

	FZoneSerialData& ZoneSerialData = GetCurentZoneSerialData();

	FZoneActorTransform SpawnTrans;
	SpawnTrans.m_nType = EActorType::Npc;
	SpawnTrans.m_IDEntity = data.RowName;
	SpawnTrans.m_SpawnPosition = ResultPos;
	SpawnTrans.m_SpawnRotation = FRotator(0,FMath::RandRange(0, 360),0);

	ZoneSerialData.m_AryZoneActorTrans.Add(SpawnTrans);
	
	int LastIndex = ZoneSerialData.m_AryZoneActorTrans.Num() - 1;
	
	return SpawnNpcActor(SpawnTrans,LastIndex, ZoneSerialData);
}

void UZoneInstManager::SpawnActors(const FZoneSerialData& zoneInst, bool isInit)
{
	int Index = 0;
	
	for(const FZoneActorTransform& ZoneActorEle : zoneInst.m_AryZoneActorTrans)
	{
		switch (ZoneActorEle.m_nType)
		{
		case EActorType::Npc:
			{
				AMonsterPawn* MobPawn = SpawnNpcActor(ZoneActorEle, Index, zoneInst);
			}
			break;
		case EActorType::Item:
			{
				AItemActor* ItemActor = SpawnItemActor(ZoneActorEle, Index, zoneInst, isInit);
			}
			break;
		case EActorType::Gather:
			{
				ATreeBase* GatherActor = SpawnGatherActor(ZoneActorEle, Index, zoneInst, isInit);
			}
			break;
		case EActorType::Build:
			{
				AStructureActor* BuildActor = SpawnBuildActor(ZoneActorEle, Index, zoneInst);
			}
			break;
		}
		Index++;
	}
}

FZoneSerialData& UZoneInstManager::GetCurentZoneSerialData()
{
	FName CrntZoneID = UMyGameInstance::Get->m_LevelMoveManager->GetCrntZoneID();
	
	FZoneSerialData* BuildInst = m_MapBuildInsts.Find(CrntZoneID);
	
	return *BuildInst;
}

void UZoneInstManager::AddPlayerAllItem(UInventory* inven)
{
	UEquipManager* Equip = UMyLib::GetEquip();
	
	TArray<FItemSpec> AryTotalItems;
	
	AryTotalItems.Reserve(100);

	if(!Equip->GetQuickSlotItem().m_ID.IsNone())
	{
		AryTotalItems.Add(Equip->GetQuickSlotItem());
	}

	if(Equip->GetBag())
	{
		for(const FItemSpec& ItemInInven : Equip->GetBag()->GetAryItems())
		{
			if(ItemInInven.m_ID.IsNone())
			{
				continue;
			}
			AryTotalItems.Add(ItemInInven);
		}
	}

	if(Equip->GetBelt())
	{
		for(const FItemSpec& ItemInInven : Equip->GetBelt()->GetAryItems())
		{
			if(ItemInInven.m_ID.IsNone())
			{
				continue;
			}
			AryTotalItems.Add(ItemInInven);
		}
	}

	for(const FItemSpec& ItemInInven : UMyLib::GetPlayerInven()->GetAryItems())
	{
		if(ItemInInven.m_ID.IsNone())
		{
			continue;
		}
		AryTotalItems.Add(ItemInInven);
	}

	for(int i = 0; i < (int)EEquipSlotType::Length; i++)
	{
		const FItemSpec& ItemInEquip = Equip->GetEquipAry()[i];
		
		if(ItemInEquip.m_ID.IsNone())
		{
			continue;
		}
		AryTotalItems.Add(ItemInEquip);
	}
	
	inven->Init(AryTotalItems.Num(),NSLOCTEXT("UZoneInstManager","Player Tomb","플레이어 시체"));

	for(FItemSpec& ItemToAdd : AryTotalItems)
	{
		inven->AddItem(ItemToAdd,false);
	}
}

void UZoneInstManager::ResetZone()
{
	TArray<FName> AryRemove;
	
	AryRemove.Reserve(10);
	
	for(const auto& ZoneData : m_MapBuildInsts)
	{
		if((!m_PlayerTombZoneID.IsNone() && ZoneData.Key == m_PlayerTombZoneID) || ZoneData.Key == TEXT("PlayerHome"))
		{
			continue;
		}
		AryRemove.Add(ZoneData.Key);
	}

	for(FName RemoveKey : AryRemove)
	{
		m_MapBuildInsts.Remove(RemoveKey);
	}
}

TArray<FZoneActorTransform> UZoneInstManager::CreateBuildInst(const UNPCPaletteDataAsset* npcAssets)
{
	TArray<FZoneActorTransform> AryZones;

	if(!npcAssets)
	{
		return AryZones;
	}
	for(const FNPCSpawnData& SpawnData : npcAssets->m_ArySpawnDatas)
	{
		FZoneActorTransform NewBuildInst;
		
		if(SpawnData.m_EntityParentTable->RowStruct->IsChildOf(FNpcUnitEntityRow::StaticStruct()))
		{
			NewBuildInst.m_nType = EActorType::Npc;
		}
		else if(SpawnData.m_EntityParentTable->RowStruct->IsChildOf(FItemDataRow::StaticStruct()))
		{
			NewBuildInst.m_nType = EActorType::Item;
		}
		else if(SpawnData.m_EntityParentTable->RowStruct->IsChildOf(FGatherDataRow::StaticStruct()))
		{
			NewBuildInst.m_nType = EActorType::Gather;
		}
		else if(SpawnData.m_EntityParentTable->RowStruct->IsChildOf(FBuildDataRow::StaticStruct()))
		{
			NewBuildInst.m_nType = EActorType::Build;
		}
		NewBuildInst.m_IDEntity = SpawnData.m_IDEntity;
		
		NewBuildInst.m_SpawnPosition = SpawnData.m_SpawnPosition;
		
		NewBuildInst.m_SpawnRotation = SpawnData.m_SpawnRotation;

		AryZones.Add(NewBuildInst);
	}
	return AryZones;
}

AMonsterPawn* UZoneInstManager::SpawnNpcActor(const FZoneActorTransform& spawnData, int index, const FZoneSerialData& serialData)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FNpcUnitEntityRow* EntityRow = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(spawnData.m_IDEntity, "");
	
	AMonsterPawn* NpcActor = UMyLib::GetUWorld()->SpawnActor<AMonsterPawn>(EntityRow->m_ClassActor, spawnData.m_SpawnPosition, spawnData.m_SpawnRotation, Param);
	NpcActor->SetEntity(spawnData.m_IDEntity,*EntityRow);

	const int* HpPtr = serialData.m_MapGatherHp.Find(index);
	if(HpPtr)
	{
		NpcActor->SetHp(*HpPtr);
		if(*HpPtr <= 0)
		{
			NpcActor->SetDeadBody();
		}
	}
	
	auto* ItemHolder = serialData.m_MapItemHolders.Find(index);
	if(ItemHolder)
	{
		NpcActor->SetInven(ItemHolder->Get());
	}
	else
	{
		NpcActor->SetInven(nullptr);
	}

	m_Npc.Add(NpcActor);

	return NpcActor;
}

AItemActor* UZoneInstManager::SpawnItemActor(const FZoneActorTransform& spawn_data, int index, const FZoneSerialData& serialData, bool isInit)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FItemDataRow* EntityRow = UItemData::GetItemTable->FindRow<FItemDataRow>(spawn_data.m_IDEntity, "");

	FRotator Rot = spawn_data.m_SpawnRotation;

	if (isInit)
	{
		Rot += FRotator(0, FMath::RandRange(0, 360), 0);
	}
	AItemActor* ItemActor = UMyLib::GetUWorld()->SpawnActor<AItemActor>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, Rot, Param);

	ItemActor->Init(spawn_data.m_IDEntity,1);

	m_Item.Add(ItemActor);

	UMyGameInstance::Get->m_ZoneInst->AddFocusActor(ItemActor);

	return ItemActor;
}

ATreeBase* UZoneInstManager::SpawnGatherActor(const FZoneActorTransform& spawn_data, int index, const FZoneSerialData& serialData, bool isInit)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FGatherDataRow* EntityRow = UGatherTable::GetGatherTable->FindRow<FGatherDataRow>(spawn_data.m_IDEntity, "");

	FRotator Rot = spawn_data.m_SpawnRotation;
	
	if(isInit)
	{
		Rot += FRotator(0,FMath::RandRange(0,360),0);
	}
	
	ATreeBase* TreeActor = UMyLib::GetUWorld()->SpawnActor<ATreeBase>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, Rot, Param);

	const int* HpPtr = serialData.m_MapGatherHp.Find(index);
	
	TreeActor->SetEntity(spawn_data.m_IDEntity, *EntityRow,UMyLib::GetPlayer(), HpPtr);
	
	m_Gather.Add(TreeActor);

	UMyGameInstance::Get->m_ZoneInst->AddFocusActor(TreeActor);

	return TreeActor;
}
AStructureActor* UZoneInstManager::SpawnBuildActor(const FZoneActorTransform& spawn_data, int index, const FZoneSerialData& serialData)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FBuildDataRow* EntityRow = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(spawn_data.m_IDEntity, "");
	
	AStructureActor* StructureActor = UMyLib::GetUWorld()->SpawnActor<AStructureActor>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation, Param);

	StructureActor->SetBuildData(*EntityRow);
	
	auto* InvenFound = serialData.m_MapItemHolders.Find(index);
	
	StructureActor->ConfirmBuild( InvenFound ? InvenFound->Get() : nullptr);

	if(UMyGameInstance::Get->m_LevelMoveManager->GetCrntZoneID() == TEXT("PlayerHome"))
	{
		UMyGameInstance::Get->m_BuildManager->SetStructureGrid(StructureActor);
	}

	AddBuildActor(StructureActor);
	
	return StructureActor;
}

void UZoneInstManager::SpawnPlayer(const FVector& loc, const FRotator& rot)
{
	AMyPlayerPawn* Pl = UMyLib::GetPlayer();
	
	float Diff = Pl->GetCapsule()->GetScaledCapsuleHalfHeight();

	FVector LocNew = loc;

	LocNew.Z += Diff;

	Pl->SetActorLocation(LocNew, true);
	
	Pl->SetActorRotation(rot);
}

void UZoneInstManager::AddFocusActor(UObject* want)
{
	TScriptInterface<IFocusable> Focus;
	Focus.SetInterface(want);
	Focus.SetObject(want);
	m_AryFocusActors.Add(Focus);
}

void UZoneInstManager::RemoveFocusActor(UObject* want)
{
	TScriptInterface<IFocusable> Focus;
	Focus.SetInterface(want);
	Focus.SetObject(want);
	m_AryFocusActors.Remove(Focus);
}

IFocusable* UZoneInstManager::GetNearProp(FVector callerLoc, float range, UClass* ignoreClass, bool excludeNotInteractable)
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
		if (!Focus.GetObject())
		{
			continue;
		}

		AActor* FocusActor = Cast<AActor>(Focus.GetObject());

		if(FocusActor->GetClass()->IsChildOf(ignoreClass))
		{
			continue;
		}

		IFocusable* FocusInter = Cast<IFocusable>(Focus.GetObject());

		if(excludeNotInteractable && !FocusInter->IsInteractable())
		{
			continue;//벨수 없는 나무를 스킵한다 
		}
		
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
			NearPawn = FocusInter;

			MAX_Dist = Length;
		}
	}

	return NearPawn;
}

IFocusable* UZoneInstManager::GetNearTarget(FVector callerLoc, float range, bool isUseAuto)
{
	bool ExcludeNotInteractable = false;
	
	bool ExcludeDead = false;
	
	UClass* IgnoreClass = nullptr;
	
	if(isUseAuto)
	{
		IgnoreClass = AStructureActor::StaticClass();
		ExcludeDead = true;
		ExcludeNotInteractable = true;
		range = 0;
	}
	ACombatUnitPawn* Pawn = GetNearNpc(callerLoc, range, nullptr, ExcludeDead);
	
	IFocusable* Prop = GetNearProp(callerLoc, range, IgnoreClass, ExcludeNotInteractable);
	
	AActor* FocusActor = Cast<AActor>(Prop);

	if(Pawn && FocusActor)
	{
		FVector Loc1 = Pawn->GetActorLocation();
		
		FVector Loc2 = FocusActor->GetActorLocation();

		float Dist1 = FVector::DistSquared2D(Loc1, callerLoc);

		float Dist2 = FVector::DistSquared2D(Loc2, callerLoc) + (Pawn->IsAlive() ? 250000 : 0 );

		float ARange = UMyLib::GetPlayer()->GetAttackRangeSqr(); 
		
		if(Dist1 <= ARange ||  Dist1 <= Dist2)
		{
			return Pawn;
		}
		return Prop;
	}
	else if (Pawn)
	{
		return Pawn;		
	}
	return Prop;
}
ACombatUnitPawn* UZoneInstManager::GetNearNpc(FVector callerLoc, float range, const TSet<ACombatUnitPawn*>* ignore, bool excludeDead)
{
	float MAX_Dist = MAX_flt;

	range = range * range;

	ACombatUnitPawn* NearPawn = nullptr;

	AMyPlayerPawn* PlayerPawn = UMyLib::GetPlayer();

	FNavLocation StartPoint;

	UNavigationSystemV1* NavSys = UMyLib::GetNavSys();

	NavSys->ProjectPointToNavigation(callerLoc, StartPoint);

	for (TWeakObjectPtr<AMonsterPawn>& Pawn : m_Npc)
	{
		if (!Pawn.Get() || (excludeDead && !Pawn->IsAlive()) || Pawn->IsHidden() || (ignore && (*ignore).Contains(Pawn.Get())) || !Pawn->IsFocusable())
		{
			continue;
		}
		
		float Length = MAX_flt;

		FNavLocation EndPoint;

		NavSys->ProjectPointToNavigation(Pawn->GetActorLocation(), EndPoint);

		if (!PlayerPawn->LineOfSightTo(Pawn.Get()))
		{
			NavSys->GetPathLength(GetWorld(), StartPoint, EndPoint, Length);

			Length = Length * Length;
		}
		else
		{
			Length = FVector::DistSquared2D(StartPoint, EndPoint);
		}

		if (range > 0 && range < Length)//900, 실제 거리 1300
		{
			continue;
		}

		if(!Pawn->IsAlive())
		{
			Length += 562500;
		}
		
		if (MAX_Dist > Length)
		{
			NearPawn = Pawn.Get();
			MAX_Dist = Length;
		}
	}
	return NearPawn;
}

void UZoneInstManager::GetNearNpcs(const ABaseUnitPawn* caller, TArray<ACombatUnitPawn*>& outAry, float range,const TSet<ACombatUnitPawn*>* ignore)
{
	FVector Loc = caller->GetActorLocation();

	range = range * range;

	FNavLocation StartPoint;

	UNavigationSystemV1* NavSys = UMyLib::GetNavSys();

	NavSys->ProjectPointToNavigation(Loc, StartPoint);

	for (TWeakObjectPtr<AMonsterPawn>& Pawn : m_Npc)
	{
		if (!Pawn.Get() || !Pawn->IsAlive() || Pawn->IsHidden() || (ignore && (*ignore).Contains(Pawn.Get())))
		{
			continue;
		}

		float Length = MAX_flt;

		FNavLocation EndPoint;

		NavSys->ProjectPointToNavigation(Pawn->GetActorLocation(), EndPoint);

		if (!caller->LineOfSightTo(Pawn.Get()))
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

		outAry.Add(Pawn.Get());
	}
}

void UZoneInstManager::AddTrackIcon(IFocusable* icon)
{
	m_MiniMapCam->AddTrackIcon(icon);
}

void UZoneInstManager::RemovePlayerTomb()
{
	if(!m_PlayerTombZoneID.IsNone())
	{
		int Index = 0;
		
		if(m_PlayerTombZoneID == UMyGameInstance::Get->m_LevelMoveManager->GetCrntZoneID())
		{
			for(TWeakObjectPtr<AStructureActor> CurrentBuildActor : m_Build)
			{
				if(CurrentBuildActor->GetBuildData().m_RowID == TEXT("PlayerTomb"))
				{
					break;
				}
				Index++;
			}
			m_Build.RemoveAt(Index);
		}
		
		Index = 0;
		
		FZoneSerialData* PlZOneData = m_MapBuildInsts.Find(m_PlayerTombZoneID);
		
		for(const FZoneActorTransform& ZoneTans : PlZOneData->m_AryZoneActorTrans)
		{
			if(ZoneTans.m_IDEntity == TEXT("PlayerTomb"))
			{
				break;
			}
			Index++;
		}
		PlZOneData->m_MapItemHolders.Remove(Index);

		PlZOneData->m_AryZoneActorTrans.RemoveAt(Index);
	}
	m_PlayerTombZoneID = NAME_None;
}

void UZoneInstManager::SaveActorsOnPlayerDead(const FName& id)
{
	RemovePlayerTomb();

	m_PlayerTombZoneID = id;
	
	UInventory* DeadInven = NewObject<UInventory>(UMyGameInstance::Get);
	
	AddPlayerAllItem(DeadInven);
	
	if(DeadInven->IsInvenEmpty())
	{
		return;
	}

	int Index = SaveActors(id);

	AActor* Pl = UMyLib::GetPlayer();

	FZoneSerialData& ZoneSerialData = GetCurentZoneSerialData();

	FZoneActorTransform ZoneData;
	ZoneData.m_nType = EActorType::Build;
	ZoneData.m_IDEntity = TEXT("PlayerTomb");
	ZoneData.m_SpawnPosition = Pl->GetActorLocation();
	ZoneData.m_SpawnRotation = FRotator(0,0,0);
	ZoneSerialData.m_AryZoneActorTrans.Add(ZoneData);
	
	TStrongObjectPtr<UInventory> ItemHolder(DeadInven);
	ZoneSerialData.m_MapItemHolders.Add(Index, ItemHolder);	
}
