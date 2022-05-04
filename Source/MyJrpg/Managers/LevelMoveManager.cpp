#include "LevelMoveManager.h"

#include "BUITween.h"
#include "ConstructionManager.h"
#include "EquipManager.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ZoneData.h"
//지금부터 레벨 스티리밍없앨거임
//첫로드는 게이트 정보 필요없음
void ULevelMoveManager::StartGame()
{
	m_bIsGameStart = true;
	
	OpenMyLevel(TEXT("PlayerHome"));
}

void ULevelMoveManager::OpenMyLevel(const FZoneDataRow& zoneData)
{
	m_ZoneData = &zoneData;

	UGameplayStatics::OpenLevel(this,m_ZoneData->m_MapName);
}

void ULevelMoveManager::OpenMyLevel(FName zoneData)
{
	UMyGameInstance::Get->m_GameRule = nullptr;
	
	const FZoneDataRow* DataRow = UZoneData::GetZoneTable->FindRow<FZoneDataRow>(zoneData, "");
	
	OpenMyLevel(*DataRow);
}

void ULevelMoveManager::OnOpenWorldLevelComplete()
{
	UMyAssetManager::Get()->ClearUnits();
	
	UBUITween::Shutdown();

	if(m_ZoneData->m_ClassGameRule->IsValidLowLevel())
	{
		UMyGameInstance::Get->m_GameRule = NewObject<UGameRuleBase>(this, m_ZoneData->m_ClassGameRule);

	}
	UMyGameInstance::Get->m_SpawnManager->SetSpawnActors(m_ZoneData->m_SpawnDataNpc);

	UMyGameInstance::Get->m_EquipManager->UpdateEquip();

	FText tLevelName = m_ZoneData->m_ShowingName;
	
	m_OnLvelMoveComp.Broadcast(GetCrntZoneID());

	m_OnLvelMoveCompText.Broadcast(tLevelName);

	if(GetCrntZoneID() == TEXT("PlayerHome"))
	{
		UMyGameInstance::Get->m_BuildManager->LoadConstruction();
	}
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
