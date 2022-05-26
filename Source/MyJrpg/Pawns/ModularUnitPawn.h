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


struct FItemSpec;
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
	
	FSkeletalMeshMergeParams m_MergeParam;
	
protected:
	bool m_bIsHatEquipped;

	bool m_bIsGloveEquipped;

	bool m_bIsBootHighEquipped;

protected:
	UPROPERTY()
	UStaticMesh* m_CacheLeftHand;
	UPROPERTY()
	UStaticMesh* m_CacheRightHand;
	UPROPERTY()
	UStaticMesh* m_Pickaxe;
	UPROPERTY()
	UStaticMesh* m_Axe;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void UpdateMorpthTarget();
	
	void TrySpawnBullets(const FItemDataRow& Itemdata);
	
	void TryKillBullets();

	void AttachWeapons();

	virtual void LoadSetSkMeshAnim(TSoftObjectPtr<UUnitEntityAsset> asset) override;
	
protected:
	void UpdateEquipActor();

	void SpawnEquipActor(const FWeaponData& weaponData);

	virtual UAnimMontage* GetBaseAttackMontage() override;

public:
	void ShowWeapon();

	void HideWeapon();

	FItemSpec* TryShowPickAxe();

	FItemSpec* TryShowAxe();

	UStaticMeshComponent* GetLeftWeaponMesh() const;
	
	UStaticMeshComponent* GetRightWeaponMesh() const;
};
