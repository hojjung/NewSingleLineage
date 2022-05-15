// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularUnitPawn.h"

#include "PetPawn.h"
#include "MyJrpg/Actors/Equipments/AttachEquipmentBase.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

AModularUnitPawn::AModularUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
	m_MeshHat = CreateSkMeshComp(TEXT("m_MeshHat"));
	m_MeshHead = CreateSkMeshComp(TEXT("m_MeshHead"));
	m_MeshChest = CreateSkMeshComp(TEXT("mm_MeshChest"));
	m_MeshGloves = CreateSkMeshComp(TEXT("m_MeshGloves"));
	m_MeshLegs = CreateSkMeshComp(TEXT("m_MeshLegs"));

	m_ArySkMeshes.Reset();
	m_ArySkMeshes.Add(m_MeshHead);
	m_ArySkMeshes.Add(m_MeshHat);
	m_ArySkMeshes.Add(m_MeshChest);
	m_ArySkMeshes.Add(m_MeshGloves);
	m_ArySkMeshes.Add(m_MeshLegs);
	//
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundHead(TEXT(
		"SkeletalMesh'/Game/14_ModularArmor/MedievalArmour/CharacterParts/Meshes/Basebody/SK_ma_armour_head_01_.SK_ma_armour_head_01_'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundBody(TEXT(
		"SkeletalMesh'/Game/14_ModularArmor/MedievalArmour/CharacterParts/Meshes/Basebody/SK_ma_body_master.SK_ma_body_master'"));
	//AnimBlueprint'/Game/14_ModularArmor/Anims/ABP_Default.ABP_Default'
	m_CachedMeshHead = FoundHead.Object;
	m_CachedMeshBody = FoundBody.Object;

	static ConstructorHelpers::FClassFinder<UAnimInstance> FoundAnim(
		TEXT("AnimBlueprint'/Game/14_ModularArmor/Anims/ABP_Default.ABP_Default_C'"));
	m_ClassAnimBP = FoundAnim.Class;
}

void AModularUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	SetMasterPose();
	UpdateMorpthTarget();

	m_AryEqupActors.Init(nullptr, (int)EEquipSlotType::Length - 1);
	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &AModularUnitPawn::UpdateEquipActor);
	SetDefaultMesh();
	UpdateEquipActor();
}

void AModularUnitPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (USkeletalMeshComponent* Sk : m_ArySkMeshes)
	{
		Sk->SetSkeletalMesh(nullptr);
	}
}

void AModularUnitPawn::SetMasterPose()
{
	int Iter = -1;
	while (++Iter < m_ArySkMeshes.Num())
	{
		m_ArySkMeshes[Iter]->SetMasterPoseComponent(m_BodyMesh);
	}
}

void AModularUnitPawn::UpdateMorpthTarget()
{
	m_BodyMesh->SetMorphTarget(TEXT("hat_equipped"), m_bIsHatEquipped ? 1.f : 0.f);
	m_BodyMesh->SetMorphTarget(TEXT("gloves_equipped"), m_bIsGloveEquipped ? 1.f : 0.f);
	m_BodyMesh->SetMorphTarget(TEXT("boot_high_equipped"), m_bIsBootHighEquipped ? 1.f : 0.f);
	
	int Iter = -1;
	while (++Iter < m_ArySkMeshes.Num())
	{
		m_ArySkMeshes[Iter]->SetMorphTarget(TEXT("hat_equipped"), m_bIsHatEquipped ? 1.f : 0.f);
		m_ArySkMeshes[Iter]->SetMorphTarget(TEXT("gloves_equipped"), m_bIsGloveEquipped ? 1.f : 0.f);
		m_ArySkMeshes[Iter]->SetMorphTarget(TEXT("boot_high_equipped"), m_bIsBootHighEquipped ? 1.f : 0.f);
	}
}

USkeletalMeshComponent* AModularUnitPawn::GetModuleSkMesh(EBodyIndex t)
{
	return GetModuleSkMesh((int)t);
}

USkeletalMeshComponent* AModularUnitPawn::GetModuleSkMesh(int t)
{
	return m_ArySkMeshes[t];
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

void AModularUnitPawn::SetDefaultMesh()
{
	GetModuleSkMesh(EBodyIndex::Head)->SetSkeletalMesh(m_CachedMeshHead);
	m_BodyMesh->SetSkeletalMesh(m_CachedMeshBody);
	m_BodyMesh->SetAnimClass(m_ClassAnimBP);
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

			GetModuleSkMesh(Iter)->SetSkeletalMesh(Itemdata.m_ArmorMesh.LoadSynchronous());
		}
		else
		{
			GetModuleSkMesh(Iter)->SetSkeletalMesh(nullptr);
		}
	}
}

void AModularUnitPawn::SpawnEquipActor(int indexSlot, TSubclassOf<AAttachEquipmentBase> classEquipActor)
{
	if (m_AryEqupActors[indexSlot])
	{
		if (m_AryEqupActors[indexSlot]->GetClass() == classEquipActor)
		{
			return;
		}
		m_AryEqupActors[indexSlot]->Destroy();
	}

	FActorSpawnParameters Params;

	Params.bNoFail = true;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAttachEquipmentBase* SpawnActor = GetWorld()->SpawnActor<AAttachEquipmentBase>(
		classEquipActor, FVector(0), FRotator(0), Params);

	FName SocketName = SpawnActor->GetSocketName();

	m_AryEqupActors[indexSlot] = SpawnActor;

	SpawnActor->AttachToComponent(GetSkMesh(),
	                              FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), SocketName);
}

void AModularUnitPawn::ShowWeapon()
{
	int WeapIndex = (int)(EEquipSlotType::Weapon) - 1;
	if (m_AryEqupActors[WeapIndex])
		m_AryEqupActors[WeapIndex]->SetActive(true);
}

void AModularUnitPawn::HideWeapon()
{
	int WeapIndex = (int)(EEquipSlotType::Weapon) - 1;
	if (m_AryEqupActors[WeapIndex])
		m_AryEqupActors[WeapIndex]->SetActive(false);
}
