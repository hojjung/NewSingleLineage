#include "SpawnEventManager.h"

#include "MyGameInstance.h"

void USpawnEventManager::StartSpawn(const FSpawnDataRow& data)
{
	m_SpawnData = &data;

	m_nIndex = 0;

	m_fTimer = 0;
}

void USpawnEventManager::StartSpawn(const FSpawnDataRow& data, float timer, int index)
{
	StartSpawn(data);

	m_nIndex = index;

	m_fTimer = timer;
}

void USpawnEventManager::Tick(float delta)
{
	if(!m_SpawnData)
	{
		return;
	}
	if(m_SpawnData->m_UnitSpawnDatas.Num() <= m_nIndex)
	{
		m_SpawnData = nullptr;
		return;
	}
	const FUnitSpawnData& SpawnData = m_SpawnData->m_UnitSpawnDatas[m_nIndex];

	m_fTimer += delta;
	
	if(m_fTimer >= SpawnData.m_fSpawnDelay)
	{
		int Num = SpawnData.m_Units.Num();
		
		int RandIndex = FMath::RandRange(0, Num - 1);
		
		UMyGameInstance::Get->m_ZoneInst->SpawnAdditionalNpcActor(SpawnData.m_Units[RandIndex]);

		m_fTimer = 0;

		m_nIndex++;
	}
}

void USpawnEventManager::SaveSpawnEvent(FZoneSerialData& inst)
{
	inst.m_nSpawnEventIndex = m_nIndex;

	inst.m_fSpawnEventTimer = m_fTimer;
	
	m_SpawnData = nullptr;
}
