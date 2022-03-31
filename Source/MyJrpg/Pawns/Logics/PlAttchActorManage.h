// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Equipments/AttachEquipmentBase.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "MyJrpg/Pawns/PetPawn.h"
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

public:
	UPlAttchActorManage();
	
protected:
	TWeakObjectPtr<AMyPlayerPawn> m_PlOwner;
	UPROPERTY()
	TArray<AAttachEquipmentBase*> m_AryEqupActors;
	UPROPERTY()
	TSubclassOf<AAttachEquipmentBase> m_ClassEquip;
	UPROPERTY()
	APetPawn* m_Pet;

public:
	void Init(AMyPlayerPawn* my_player_pawn);
	
	void SetPet(const FPetRow& pet_row);
	
	void UnEquipPet();

protected:
	void UpdateEquipActor();

	void SpawnEquipActor(int indexSlot, TSubclassOf<AAttachEquipmentBase> classEquipActor);
};


