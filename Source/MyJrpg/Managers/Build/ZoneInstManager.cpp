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
	m_Npc.Reset();
	
	m_Item.Reset();
	
	m_Gather.Reset();
	
	m_Build.Reset();
	
	TArray<FZoneActor>* BuildInst = m_MapBuildInsts.Find(id);
	
	if (!BuildInst)
	{
		InitZone(id, zoneData);
		return ;
	}
	SpawnActors(*BuildInst);
}

void UZoneInstManager::SaveActors(const FName& id)
{
	TArray<FZoneActor>* BuildInst = m_MapBuildInsts.Find(id);

	if(!BuildInst)
	{
		return;
	}
	
	BuildInst->Reset();

	for(TWeakObjectPtr<AMonsterPawn> ActorEle : m_Npc)
	{
		if(!ActorEle.Get() ||!ActorEle->IsAlive())
		{
			continue;
		}
		FZoneActor ZoneData;
		ZoneData.m_nType = EActorType::Npc;
		ZoneData.m_IDEntity = ActorEle->GetEntityID();
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->Add(ZoneData);
	}
	for(TWeakObjectPtr<AItemActor> ActorEle : m_Item)
	{
		if(!ActorEle.Get())
		{
			continue;
		}
		FZoneActor ZoneData;
		ZoneData.m_nType = EActorType::Item;
		ZoneData.m_IDEntity = ActorEle->GetItemSpec().m_ID;
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->Add(ZoneData);
	}
	for(TWeakObjectPtr<ATreeBase> ActorEle : m_Gather)
	{
		if(!ActorEle.Get())
		{
			continue;
		}
		FZoneActor ZoneData;
		ZoneData.m_nType = EActorType::Gather;
		ZoneData.m_IDEntity = ActorEle->GetID();
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->Add(ZoneData);
	}
	for(TWeakObjectPtr<AStructureActor> ActorEle : m_Build)
	{
		if(!ActorEle.Get())
		{
			continue;
		}
		FZoneActor ZoneData;
		ZoneData.m_nType = EActorType::Build;
		ZoneData.m_IDEntity = ActorEle->GetBuildData().m_RowID;
		ZoneData.m_SpawnPosition = ActorEle->GetActorLocation();
		ZoneData.m_SpawnRotation = ActorEle->GetActorRotation();
		BuildInst->Add(ZoneData);
	}
	//나무 체력
	//NPC인벤토리
	//창고 인벤토리
	//가구가 하고있는 슬롯
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

	UMyGameInstance::Get->m_SpawnManager->AddFocusActor(NpcActor);

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

	m_Item.Add(ItemActor);

	UMyGameInstance::Get->m_SpawnManager->AddFocusActor(ItemActor);

	return ItemActor;
}

ATreeBase* UZoneInstManager::SpawnGatherActor(const FZoneActor& spawn_data)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FGatherDataRow* EntityRow = UGatherTable::GetGatherTable->FindRow<FGatherDataRow>(spawn_data.m_IDEntity, "");
	
	ATreeBase* TreeActor = UMyLib::GetUWorld()->SpawnActor<ATreeBase>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation + FRotator(0,FMath::RandRange(0,360),0), Param);

	TreeActor->SetEntity(spawn_data.m_IDEntity, *EntityRow,UMyLib::GetPlayer());
	
	m_Gather.Add(TreeActor);

	UMyGameInstance::Get->m_SpawnManager->AddFocusActor(TreeActor);

	return TreeActor;
}
AStructureActor* UZoneInstManager::SpawnBuildActor(const FZoneActor& spawn_data)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FBuildDataRow* EntityRow = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(spawn_data.m_IDEntity, "");
	
	AStructureActor* StructureActor = UMyLib::GetUWorld()->SpawnActor<AStructureActor>(EntityRow->m_ClassActor, spawn_data.m_SpawnPosition, spawn_data.m_SpawnRotation + FRotator(0,FMath::RandRange(-180,180),0), Param);

	StructureActor->SetBuildData(*EntityRow);
	
	StructureActor->ConfirmBuild();

	m_Build.Add(StructureActor);

	UMyGameInstance::Get->m_SpawnManager->AddFocusActor(StructureActor);

	return StructureActor;
}