#include "LevelMoveManager.h"

#include "BUITween.h"
#include "ConstructionManager.h"
#include "EquipManager.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "PreviewActorManager.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ZoneData.h"
//지금부터 레벨 스티리밍없앨거임
//첫로드는 게이트 정보 필요없음
void ULevelMoveManager::StartGame()
{
	m_ZoneData = nullptr;
	
	m_bIsGameStart = true;
	
	OpenMyLevel(TEXT("PlayerHome"));
}

void ULevelMoveManager::OpenMyLevel(FName zoneData, bool isPlayerDead)
{
	const FZoneDataRow* DataRow = UZoneData::GetZoneTable->FindRow<FZoneDataRow>(zoneData, "");
	
	OpenMyLevel(*DataRow, isPlayerDead);
}

void ULevelMoveManager::OpenMyLevel(const FZoneDataRow& zoneData, bool isPlayerDead)
{
	if(m_ZoneData)
	{
		FName ID = GetCrntZoneID();

		if(isPlayerDead)
		{
			UMyGameInstance::Get->m_ZoneInst->SaveActorsOnPlayerDead(ID);
			
			UMyLib::GetPlayerInven()->ClearAllInven();
			
			UMyLib::GetEquip()->ClearAllEquipment();
		}
		else
		{
			UMyGameInstance::Get->m_ZoneInst->SaveActors(ID);
		}
	}
	m_ZoneData = &zoneData;

	UGameplayStatics::OpenLevel(this,m_ZoneData->m_MapName);
}

void ULevelMoveManager::OnOpenWorldLevelComplete()
{
	if(!m_ZoneData)
	{
		return;
	}
	
	UMyGameInstance::Get->m_ZoneInst->SpawnZone(GetCrntZoneID(), *GetZoneDataCurrent());

	UMyGameInstance::Get->m_EquipManager->UpdateEquip();

	FText tLevelName = m_ZoneData->m_ShowingName;
	
	m_OnLvelMoveComp.Broadcast(GetCrntZoneID());

	m_OnLvelMoveCompText.Broadcast(tLevelName);

	if(GetCrntZoneID() == TEXT("PlayerHome"))
	{
		UMyGameInstance::Get->m_BuildManager->SpawnGrid();
	}
	
	UMyGameInstance::Get->m_PetManager->CreatePreviewActor();
	
	UMyGameInstance::Get->m_SkillAuto->Init();
}

bool ULevelMoveManager::IsGameStart()
{
	return m_bIsGameStart;
}

const FZoneDataRow* ULevelMoveManager::GetZoneDataCurrent() const
{
	return m_ZoneData;
}

const FName& ULevelMoveManager::GetCrntZoneID() const
{
	return m_ZoneData->m_RowKey;
}
