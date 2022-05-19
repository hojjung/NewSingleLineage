// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Items/ItemConvertInst.h"
#include "UObject/NoExportTypes.h"
#include "ItemConvertManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UItemConvertManager : public UObject
{
	GENERATED_BODY()

protected:
	TMap<TSoftObjectPtr<AStructureActor>, TSoftObjectPtr<UItemConvertInst>> m_MapItemConverters;
	
public:
	void AddStructureAndItem(AStructureActor* actor, UItemConvertInst* inst);

	void RemoveStructureAndItem(AStructureActor* actor);
	
	void Tick(float delta_time);
};