#include "LevelMoveManager.h"

#include "BUITween.h"
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
	
	OpenMyLevel(TEXT("Village"),0);
}

void ULevelMoveManager::OpenMyLevel(const FZoneDataRow& zoneData, int index)
{
	m_ZoneData = &zoneData;

	m_nStageIndex = index;

	UGameplayStatics::OpenLevel(this,m_ZoneData->m_AryZones[m_nStageIndex].m_MapName);
}

void ULevelMoveManager::OpenMyLevel(FName zoneData, int index)
{
	UMyGameInstance::Get->m_GameRule = nullptr;
	
	const FZoneDataRow* DataRow = UZoneData::GetZoneTable->FindRow<FZoneDataRow>(zoneData, "");
	
	OpenMyLevel(*DataRow,index);
}

void ULevelMoveManager::OnOpenWorldLevelComplete()
{
	UMyAssetManager::Get()->ClearUnits();
	
	UBUITween::Shutdown();

	if(m_ZoneData->m_ClassGameRule->IsValidLowLevel())
	{
		UMyGameInstance::Get->m_GameRule = NewObject<UGameRuleBase>(this, m_ZoneData->m_ClassGameRule);

	}
	UMyGameInstance::Get->m_SpawnManager->SetSpawnActors(m_ZoneData->m_AryZones[m_nStageIndex].m_SpawnDataNpc);

	UMyGameInstance::Get->m_EquipManager->UpdateEquip();

	FText tLevelName;

	if (m_ZoneData->m_AryZones.Num() > 1)
	{
		FString Str = FString::Printf(TEXT("%s %d층"),*m_ZoneData->m_ShowingName.ToString(),m_nStageIndex + 1);

		tLevelName = FText::FromString(Str);
	}
	else
	{
		tLevelName = m_ZoneData->m_ShowingName;
	}
	
	m_OnLvelMoveComp.Broadcast(tLevelName);
}

bool ULevelMoveManager::IsGameStart()
{
	return m_bIsGameStart;
}

const FZoneDataRow* ULevelMoveManager::GetZoneDataCurrent() const
{
	return m_ZoneData;
}

const FDropDataRow* ULevelMoveManager::GetDropData() const
{
	const FDropDataRowHandle& DropTable = GetZoneDataCurrent()->m_AryZones[m_nStageIndex].m_DropHandle;

	if (DropTable.RowName == NAME_None)
	{
		return nullptr;
	}
	
	return DropTable.GetRow<FDropDataRow>("");
}

