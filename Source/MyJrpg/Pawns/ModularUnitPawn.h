// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatUnitPawn.h"
#include "MyJrpg/MeshMergeLib.h"
#include "ModularUnitPawn.generated.h"

/*
NPC도 이걸 사용할 수 있음
그렇다면 몬스터 스폰 시스템 또한 변경되야함
뇌절치지 말고 플레이어로 퉁처라

Preview
*/


struct FPetRow;
class APetPawn;
UENUM()
enum class EBodyIndex: uint8
{
	Head,
	Hat,
	Body,
	Gloves,
	Legs,
	Len UMETA(Hidden)
};
UCLASS()
class MYJRPG_API AModularUnitPawn : public ACombatUnitPawn
{
	GENERATED_BODY()

public:
	AModularUnitPawn(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshLeftHand;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshRightHand;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_MeshHead;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_MeshHat;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_MeshChest;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_MeshGloves;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_MeshLegs;
	UPROPERTY(VisibleAnywhere)
	TArray<USkeletalMeshComponent*> m_ArySkMeshes;
	
	FSkeletalMeshMergeParams m_MergeParam;
protected:
	UPROPERTY()
	APetPawn* m_Pet;
	UPROPERTY()
	USkeletalMesh* m_CachedMeshBody;
	UPROPERTY()
	TSubclassOf<UAnimInstance> m_ClassAnimBP;
	
protected:
	bool m_bIsHatEquipped;

	bool m_bIsGloveEquipped;

	bool m_bIsBootHighEquipped;

	EStanceType m_Stance;
	
protected:
	UPROPERTY()
	TArray<UAnimMontage*> m_BaseAttack;
	UPROPERTY()
	USkeletalMesh* m_DefaultHead;
	UPROPERTY()
	USkeletalMesh* m_DefaultBody;
	UPROPERTY()
	UStaticMesh* m_CacheLeftHand;
	UPROPERTY()
	UStaticMesh* m_CacheRightHand;
	UPROPERTY()
	UStaticMesh* m_Pickaxe;
	UPROPERTY()
	UStaticMesh* m_Axe;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void SetMasterPose();

	void UpdateMorpthTarget();

protected:
	void SetDefaultMesh();
	
	void UpdateEquipActor();

	void SpawnEquipActor(const FWeaponData& weaponData);

	virtual UAnimMontage* GetBaseAttackMontage() override;

public:
	USkeletalMeshComponent* GetModuleSkMesh(EBodyIndex t);
	
	USkeletalMeshComponent* GetModuleSkMesh(int t);

public:
	void SetPet(const FPetRow& pet_row);
	
	void UnEquipPet();

public:
	void ShowWeapon();

	void HideWeapon();

	void TryShowPickAxe();

	void TryShowAxe();

	EStanceType GetStance();
};
