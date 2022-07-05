 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Build/ZoneInstManager.h"
#include "MyJrpg/DataTables/SpawnEventTable.h"
#include "UObject/NoExportTypes.h"
#include "SpawnEventManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USpawnEventManager : public UObject
{
	GENERATED_BODY()

protected:
	const FSpawnDataRow* m_SpawnData;

	int m_nIndex;

	float m_fTimer;
	
public:
	void StartSpawn(const FSpawnDataRow& data);

	void StartSpawn(const FSpawnDataRow& data, float timer, int index);

	void Tick(float delta);
	
	void SaveSpawnEvent(FZoneSerialData& inst);
};
