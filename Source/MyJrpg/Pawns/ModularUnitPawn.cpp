// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularUnitPawn.h"

#include "PetPawn.h"
#include "MyJrpg/MeshMergeLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/PreviewActorManager.h"

AModularUnitPawn::AModularUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundBody(TEXT(
		"SkeletalMesh'/Game/14_ModularArmor/MedievalArmour/CharacterParts/Meshes/Basebody/SKEL_FullBody.SKEL_FullBody'"));
	//AnimBlueprint'/Game/14_ModularArmor/Anims/ABP_Default.ABP_Default'
	m_CachedMeshBody = FoundBody.Object;

	static ConstructorHelpers::FClassFinder<UAnimInstance> FoundAnim(
		TEXT("AnimBlueprint'/Game/14_ModularArmor/Anims/ABP_Default.ABP_Default_C'"));
	m_ClassAnimBP = FoundAnim.Class;
	//
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
	//
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack01(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/BareHand/AM_Punch.AM_Punch'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack02(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/OneHand/AM_OH.AM_OH'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack03(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/Shield/AM_Shield.AM_Shield'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack04(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/Twohand/AM_TH.AM_TH'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack05(TEXT("AAnimMontage'/Game/14_ModularArmor/Anims/Spear/AM_Spear.AM_Spear'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack06(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/Bow/AM_Bow.AM_Bow'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack07(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/Pistol/AM_Pistol.AM_Pistol'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack08(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/Rifle/AM_Rifle.AM_Rifle'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack09(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/MagicOne/AM_Magic01.AM_Magic01'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack11(TEXT("AnimMontage'/Game/14_ModularArmor/Anims/Dual/AM_Dual.AM_Dual'"));
	m_BaseAttack.Init(nullptr,(int)EStanceType::Length);
	m_BaseAttack[(int)EStanceType::None] = Attack01.Object;
	m_BaseAttack[(int)EStanceType::OneSword] = Attack02.Object;
	m_BaseAttack[(int)EStanceType::SwordShield] = Attack03.Object;
	m_BaseAttack[(int)EStanceType::TwoSword] = Attack04.Object;
	m_BaseAttack[(int)EStanceType::Spear] = Attack05.Object;
	m_BaseAttack[(int)EStanceType::Bow] = Attack06.Object;
	m_BaseAttack[(int)EStanceType::Pistol] = Attack07.Object;
	m_BaseAttack[(int)EStanceType::Rifle] = Attack08.Object;
	m_BaseAttack[(int)EStanceType::OneMagic] = Attack09.Object;
	m_BaseAttack[(int)EStanceType::Dual] = Attack11.Object;
}

void AModularUnitPawn::AttachWeapons()
{
	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
	m_MeshLeftHand->AttachToComponent(m_BodyMesh, Rules, TEXT("LeftHandSocket"));
	m_MeshRightHand->AttachToComponent(m_BodyMesh, Rules, TEXT("RightHandSocket"));
}

void AModularUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	SetDefaultMesh();
	UpdateMorpthTarget();
	AttachWeapons();
	m_MergeParam.Skeleton = m_CachedMeshBody->GetSkeleton();
	m_MergeParam.MeshesToMerge.Init(nullptr,(int)EBodyIndex::Len + 1);
	m_MergeParam.MeshesToMerge[(int)EBodyIndex::Len] = m_CachedMeshBody;

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &AModularUnitPawn::UpdateEquipActor);
;
	UMyGameInstance::Get->m_PreviewActorManager->CreatePreviewActor();
	UpdateEquipActor();
}

void AModularUnitPawn::SetDefaultMesh()
{
	//GetModuleSkMesh(EBodyIndex::Head)->SetSkeletalMesh(m_CachedMeshHead);
	m_BodyMesh->SetSkeletalMesh(m_CachedMeshBody);
	
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
	}
	
	FName WeaponID = EquippedItems[(int)EEquipSlotType::Weapon].m_ID;
	
	if(!WeaponID.IsNone())
	{
		const FItemDataRow& Itemdata = UMyLib::GetItemData(WeaponID);
		
		SpawnEquipActor(Itemdata.m_WeaponData);

		ShowWeapon();
	}
	else
	{
		m_CacheLeftHand = nullptr;

		m_CacheRightHand = nullptr;
		m_Stance = EStanceType::None;
		HideWeapon();
	}


	USkeletalMesh* SkMeshMerged = UMeshMergeLib::MergeMeshes(m_MergeParam);

	m_BodyMesh->SetSkeletalMesh(SkMeshMerged);
	
	m_BodyMesh->SetAnimClass(m_ClassAnimBP);
	
	UMyGameInstance::Get->m_PreviewActorManager->Update(this);
}

void AModularUnitPawn::SpawnEquipActor(const FWeaponData& weaponData)
{
	m_Stance = weaponData.m_Stance;
	
	m_CacheLeftHand = weaponData.m_MeshLeft.LoadSynchronous();

	m_CacheRightHand = weaponData.m_MeshRight.LoadSynchronous();
}

UAnimMontage* AModularUnitPawn::GetBaseAttackMontage()
{
	return m_BaseAttack[(int)m_Stance];
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

FName AModularUnitPawn::TryShowPickAxe()
{
	FName Id;
	
	if(UMyLib::HasPickaxe(Id))
	{
		HideWeapon();
		
		m_MeshRightHand->SetStaticMesh(m_Pickaxe);
	}
	return Id;
}

FName AModularUnitPawn::TryShowAxe()
{
	FName Id;
	
	if(UMyLib::HasAxe(Id))
	{
		HideWeapon();
		
		m_MeshRightHand->SetStaticMesh(m_Axe);
	}
	return Id;
}

EStanceType AModularUnitPawn::GetStance()
{
	return m_Stance;
}

UStaticMeshComponent* AModularUnitPawn::GetLeftWeaponMesh() const
{
	return m_MeshLeftHand;
}

UStaticMeshComponent* AModularUnitPawn::GetRightWeaponMesh() const
{
	return m_MeshRightHand;
}

void AModularUnitPawn::SetPet(const FPetRow& pet_row)
{
	UnEquipPet();

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	FVector Loc = UMyLib::GetNavSys()->GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), 400);

	m_Pet = GetWorld()->SpawnActor<APetPawn>(APetPawn::StaticClass(), Loc, FRotator(0), Param);

	m_Pet->SetPetEntity(pet_row);
}

void AModularUnitPawn::UnEquipPet()
{
	if (m_Pet)
	{
		m_Pet->Destroy();
	}
}