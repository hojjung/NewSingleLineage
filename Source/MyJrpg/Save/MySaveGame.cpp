// All right Reserve 2021 HereticByte


#include "MySaveGame.h"

#include "MyJrpg/DataTables/ItemData.h"

FString UMySaveGame::GetSaveName()
{
	return FString::Printf(TEXT("%s/%s/%s"),*m_PlayerName,*m_ChapterName,*m_ZoneName);
}

FString UMySaveGame::GetPlayTime()
{
	int TotalHours = m_PlayTime.GetTotalHours();

	int RemainMins = m_PlayTime.GetMinutes();
	
	return FString::Printf(TEXT("%03i:%02i"),TotalHours,RemainMins);
}

void UMySaveGame::SetSaveGameDefault(FTimespan& totalPlayTime,FString& playerName, FString& chapterName,
	FString& zoneName)
{
	m_PlayTime = totalPlayTime;
	
	m_PlayerName = playerName;

	m_ChapterName = chapterName;
	
	m_ZoneName = zoneName;
}

const TArray<FUnitSpec>& UMySaveGame::GetTotalAryAllies() const
{
	return m_AryTotalAlly;
}

const TArray<FString>& UMySaveGame::GetEquipAryAllies() const
{
	return m_AryEquippedAllies;
}

const TArray<FVector>& UMySaveGame::GetEquipAryLocations() const
{
	return  m_AryEquippedAllyPosition;
}

const TArray<FRotator>& UMySaveGame::GetEquipAryRotations() const
{
	return  m_AryEquippedAllyRotation;
}

const TArray<FItemSpec>& UMySaveGame::GetInventory() const
{
	return m_AryInventory;
}

FName UMySaveGame::GetZoneName()
{
	return *m_ZoneName;
}

FString UMySaveGame::GetChapterName()
{
	return m_ChapterName;
}

const FString& UMySaveGame::GetPlayerName()
{
	return m_PlayerName;
}

void UMySaveGame::SetSaveEquippedAllies(const TArray<FString>& equipAllies, const TArray<FVector>& aryLocations,
                                        const TArray<FRotator>& aryRotations)
{
	m_AryEquippedAllies = equipAllies;
	
	m_AryEquippedAllyPosition = aryLocations;

	m_AryEquippedAllyRotation = aryRotations;
}

void UMySaveGame::SetSaveTotalAllies(const TArray<FUnitSpec>& arrayUnitSpecs)
{
	m_AryTotalAlly = arrayUnitSpecs;
}

