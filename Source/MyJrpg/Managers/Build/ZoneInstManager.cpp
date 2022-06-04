#include "ZoneInstManager.h"

#include "NPCPaletteDataAsset.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/DataTables/BuildData.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UZoneInstManager::InitZone(const FName& id, const FZoneDataRow& zoneData)
{
	TArray<FZoneActor>& BuildAdded = m_MapBuildInsts.Emplace(id, CreateBuildInst(zoneData.m_SpawnDataNpc));
	
	SpawnActors(BuildAdded);
}

void UZoneInstManager::SpawnZone(const FName& id, const FZoneDataRow& zoneData)
{
	TArray<FZoneActor>* BuildInst = m_MapBuildInsts.Find(id);
	
	if (!BuildInst)
	{
		InitZone(id, zoneData);
		return ;
	}
	SpawnActors(*BuildInst);
}

void UZoneInstManager::SaveActors()
{
	//죽으면?
	//일단은 저장안하는걸로
	//창고등 안에있는 인스턴스는 어떻게 저장?
}

void UZoneInstManager::SpawnActors(const TArray<FZoneActor>& zoneInst)
{
	for(const FZoneActor& ZoneActorEle : zoneInst)
	{
		switch (ZoneActorEle.m_nType)
		{
		case EActorType::Npc:
			SpawnNpcActor(ZoneActorEle);
			break;
		case EActorType::Item:
			SpawnItemActor(ZoneActorEle);
			break;
		case EActorType::Gather:
			SpawnGatherActor(ZoneActorEle);
			break;
		case EActorType::Build:
			SpawnBuildActor(ZoneActorEle);
			break;
		}
	}
}

TArray<FZoneActor> UZoneInstManager::CreateBuildInst(const UNPCPaletteDataAsset* npcAssets)
{
	TArray<FZoneActor> AryZones;

	for(const FNPCSpawnData& SpawnData : npcAssets->m_ArySpawnDatas)
	{
		FZoneActor NewBuildInst;
		
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


AMonsterPawn* UZoneInstManager::SpawnNpcActor(const FZoneActor& SpawnData)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FNpcUnitEntityRow* EntityRow = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(SpawnData.m_IDEntity, "");
	
	AMonsterPawn* NpcActor = UMyLib::GetUWorld()->SpawnActor<AMonsterPawn>(EntityRow->m_ClassActor, SpawnData.m_SpawnPosition, SpawnData.m_SpawnRotation, Param);
	NpcActor->SetEntity(SpawnData.m_IDEntity,*EntityRow);

	if(UMyGameInstance::Get->m_GameRule)
	{
		UMyGameInstance::Get->m_GameRule->OnMonsterCreate(NpcActor);
	}

	m_Npc.Add(NpcActor);

	return NpcActor;
}

AItemActor* UZoneInstManager::SpawnItemActor(const FZoneActor& spawn_data)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FItemDataRow* EntityRow = UItemData::GetItemTable->FindRow<FItemDataRow>(spawn_data.m_IDEntity, "");
	
	AItemActor* ItemActor = UMyLib::GetUWorld()->SpawnActor<AItemActor>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation + FRotator(0,FMath::RandRange(0,360),0), Param);

	ItemActor->Init(spawn_data.m_IDEntity,1);

	//AddFocusActor(ItemActor);

	return ItemActor;
}

ATreeBase* UZoneInstManager::SpawnGatherActor(const FZoneActor& spawn_data)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FGatherDataRow* EntityRow = UGatherTable::GetGatherTable->FindRow<FGatherDataRow>(spawn_data.m_IDEntity, "");
	
	ATreeBase* GatherActor = UMyLib::GetUWorld()->SpawnActor<ATreeBase>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation + FRotator(0,FMath::RandRange(0,360),0), Param);

	GatherActor->SetEntity(*EntityRow,UMyLib::GetPlayer());
	
	//AddFocusActor(NpcActor);

	return GatherActor;
}
AStructureActor* UZoneInstManager::SpawnBuildActor(const FZoneActor& spawn_data)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FBuildDataRow* EntityRow = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(spawn_data.m_IDEntity, "");
	
	AStructureActor* NpcActor = UMyLib::GetUWorld()->SpawnActor<AStructureActor>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation + FRotator(0,FMath::RandRange(-180,180),0), Param);

	NpcActor->SetBuildData(*EntityRow);
	
	NpcActor->ConfirmBuild();

	//AddFocusActor(NpcActor);

	return NpcActor;
}