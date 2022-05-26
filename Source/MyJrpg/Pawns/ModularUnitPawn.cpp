// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularUnitPawn.h"

#include "PetPawn.h"
#include "MyJrpg/MeshMergeLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/PreviewActorManager.h"

AModularUnitPawn::AModularUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
	m_MeshLeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshLeftHand"));
	m_MeshLeftHand->SetupAttachment(m_BodyMesh);
	m_MeshLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshLeftHand->bCastDynamicShadow = false;
	m_MeshLeftHand->bAffectDynamicIndirectLighting = true;
	m_MeshLeftHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshLeftHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	//
	m_MeshRightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshRightHand"));
	m_MeshRightHand->SetupAttachment(m_BodyMesh);
	m_MeshRightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshRightHand->bCastDynamicShadow = false;
	m_MeshRightHand->bAffectDynamicIndirectLighting = true;
	m_MeshRightHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshRightHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	//
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSm01(TEXT("StaticMesh'/Game/09_SharedAnimations/Farming_And_Mining/Meshes/Axe_Tool_SM.Axe_Tool_SM'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSm02(TEXT("StaticMesh'/Game/09_SharedAnimations/Farming_And_Mining/Meshes/Pickaxe_SM.Pickaxe_SM'"));

	m_Axe = FoundSm01.Object;
	m_Pickaxe = FoundSm02.Object;
}

void AModularUnitPawn::AttachWeapons()
{
	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
	m_MeshLeftHand->AttachToComponent(m_BodyMesh, Rules, TEXT("LeftHandSocket"));
	m_MeshRightHand->AttachToComponent(m_BodyMesh, Rules, TEXT("RightHandSocket"));
}

void AModularUnitPawn::LoadSetSkMeshAnim(TSoftObjectPtr<UUnitEntityAsset> asset)
{
	Super::LoadSetSkMeshAnim(asset);
	
	UpdateMorpthTarget();
	
	AttachWeapons();
	
	m_MergeParam.Skeleton = m_EntityAsset->m_BodyMesh->GetSkeleton();
	m_MergeParam.MeshesToMerge.Init(nullptr,(int)EBodyIndex::Len + 1);
	m_MergeParam.MeshesToMerge[(int)EBodyIndex::Len] = m_EntityAsset->m_BodyMesh;

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &AModularUnitPawn::UpdateEquipActor);
	UMyGameInstance::Get->m_PreviewActorManager->CreatePreviewActor();
	UpdateEquipActor();
}

void AModularUnitPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HideWeapon();

	m_CacheLeftHand = nullptr;

	m_CacheRightHand = nullptr;

	m_MergeParam.MeshesToMerge.Empty();

	m_Stance = EStanceType::None;
}

void AModularUnitPawn::UpdateMorpthTarget()
{
	m_BodyMesh->SetMorphTarget(TEXT("hat_equipped"), m_bIsHatEquipped ? 1.f : 0.f);
	m_BodyMesh->SetMorphTarget(TEXT("gloves_equipped"), m_bIsGloveEquipped ? 1.f : 0.f);
	m_BodyMesh->SetMorphTarget(TEXT("boot_high_equipped"), m_bIsBootHighEquipped ? 1.f : 0.f);
}

void AModularUnitPawn::TrySpawnBullets(const FItemDataRow& Itemdata)
{
	if(Itemdata.m_WeaponData.m_BulletEffect)
	{
		m_Pool = NewObject<UBulletPool>(this);
		
		m_Pool->InitPool(GetStat().m_Dmg,5,Itemdata.m_WeaponData.m_BulletEffect,this,Itemdata.m_WeaponData.m_fBulletScale);
	}
}

void AModularUnitPawn::TryKillBullets()
{
	if(m_Pool)
	{
		m_Pool->KillAll();
	}
}

void AModularUnitPawn::UpdateEquipActor()
{
	const FItemSpec* EquippedItems = UMyGameInstance::Get->m_EquipManager->GetEquipAry();

	int Iter = 0;
	while (++Iter < (int)EBodyIndex::Len)
	{
		FName ID = EquippedItems[Iter + 1].m_ID;

		if (!ID.IsNone())
		{
			const FItemDataRow& Itemdata = UMyLib::GetItemData(ID);

			m_MergeParam.MeshesToMerge[Iter] = Itemdata.m_ArmorMesh.LoadSynchronous();
		}
		else
		{
			m_MergeParam.MeshesToMerge[Iter] = nullptr;
		}
		switch ((EBodyIndex)Iter)
		{
		case EBodyIndex::Hat:
			m_bIsHatEquipped = m_MergeParam.MeshesToMerge[Iter] != nullptr;
			break;
		case EBodyIndex::Gloves:
			m_bIsGloveEquipped = m_MergeParam.MeshesToMerge[Iter] != nullptr;
			break;
		case EBodyIndex::Legs:
			m_bIsBootHighEquipped = m_MergeParam.MeshesToMerge[Iter] != nullptr;
			break;
		}
	}
	
	FName WeaponID = EquippedItems[(int)EEquipSlotType::Weapon].m_ID;
	
	if(!WeaponID.IsNone())
	{
		const FItemDataRow& Itemdata = UMyLib::GetItemData(WeaponID);
		
		SpawnEquipActor(Itemdata.m_WeaponData);

		TrySpawnBullets(Itemdata);

		ShowWeapon();

		SetAttackRange(Itemdata.m_WeaponData.m_fRange);
	}
	else
	{
		m_CacheLeftHand = nullptr;

		m_CacheRightHand = nullptr;
		m_Stance = EStanceType::None;
		HideWeapon();
		
		TryKillBullets();

		SetAttackRange(200);
	}


	USkeletalMesh* SkMeshMerged = UMeshMergeLib::MergeMeshes(m_MergeParam);

	m_BodyMesh->SetSkeletalMesh(SkMeshMerged,false);
	
	UMyGameInstance::Get->m_PreviewActorManager->Update(this);
	UpdateMorpthTarget();
}

void AModularUnitPawn::SpawnEquipActor(const FWeaponData& weaponData)
{
	m_Stance = weaponData.m_Stance;
	
	m_CacheLeftHand = weaponData.m_MeshLeft.LoadSynchronous();

	m_CacheRightHand = weaponData.m_MeshRight.LoadSynchronous();
}

UAnimMontage* AModularUnitPawn::GetBaseAttackMontage()
{
	UHumanAsset* HumanAsset = Cast<UHumanAsset>(m_EntityAsset.Get());
	
	return HumanAsset->GetStanceAnim(m_Stance);
}

void AModularUnitPawn::ShowWeapon()
{
	m_MeshLeftHand->SetStaticMesh(m_CacheLeftHand);

	m_MeshRightHand->SetStaticMesh(m_CacheRightHand);
}

void AModularUnitPawn::HideWeapon()
{
	m_MeshLeftHand->SetStaticMesh(nullptr);

	m_MeshRightHand->SetStaticMesh(nullptr);
}

FItemSpec* AModularUnitPawn::TryShowPickAxe()
{
	FItemSpec* itemSpec = nullptr;

	itemSpec = UMyLib::FindItemAllInven(TEXT("Pickaxe01"), 0);
	if(itemSpec)
	{
		HideWeapon();
		
		m_MeshRightHand->SetStaticMesh(m_Pickaxe);

		return itemSpec;
	}
	itemSpec = UMyLib::FindItemAllInven(TEXT("Pickaxe02"), 0);
	if(itemSpec)
	{
		HideWeapon();
	
		m_MeshRightHand->SetStaticMesh(m_Pickaxe);
	}
	return itemSpec;
}

FItemSpec* AModularUnitPawn::TryShowAxe()
{
	FItemSpec* itemSpec = nullptr;

	itemSpec = UMyLib::FindItemAllInven(TEXT("Axe01"), 0);
	if(itemSpec)
	{
		HideWeapon();
		
		m_MeshRightHand->SetStaticMesh(m_Axe);

		return itemSpec;
	}
	itemSpec = UMyLib::FindItemAllInven(TEXT("Axe02"), 0);
	if(itemSpec)
	{
		HideWeapon();
	
		m_MeshRightHand->SetStaticMesh(m_Axe);
	}
	return itemSpec;
}

UStaticMeshComponent* AModularUnitPawn::GetLeftWeaponMesh() const
{
	return m_MeshLeftHand;
}

UStaticMeshComponent* AModularUnitPawn::GetRightWeaponMesh() const
{
	return m_MeshRightHand;
}