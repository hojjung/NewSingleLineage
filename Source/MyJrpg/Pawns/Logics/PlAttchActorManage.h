// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Equipments/AttachEquipmentBase.h"
#include "UObject/NoExportTypes.h"
#include "PlAttchActorManage.generated.h"

class AMyPlayerPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API UPlAttchActorManage : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<AAttachEquipmentBase*> m_AryEqupActors;
	UPROPERTY()
	TSubclassOf<AAttachEquipmentBase> m_ClassEquip;
	
	TWeakObjectPtr<AMyPlayerPawn> m_PlOwner; 
public:
	UPlAttchActorManage();
	
	void Init(AMyPlayerPawn* my_player_pawn);

protected:
	void UpdateEquipActor();

	void SpawnEquipActor(int indexSlot, TSubclassOf<AAttachEquipmentBase> classEquipActor);
};


