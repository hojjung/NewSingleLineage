#include "PlAttchActorManage.h"

#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"


UPlAttchActorManage::UPlAttchActorManage()
{
	static ConstructorHelpers::FClassFinder<AAttachEquipmentBase>
	FoundEquip(TEXT("Blueprint'/Game/Blueprints/EquipActor/BP_DefaultSword.BP_DefaultSword_C'"));
	m_ClassEquip = FoundEquip.Class;
}

void UPlAttchActorManage::Init(AMyPlayerPawn* my_player_pawn)
{
	m_PlOwner = my_player_pawn;
	
	m_AryEqupActors.Init(nullptr,(int)EEquipSlotType::Length-1);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this,&UPlAttchActorManage::UpdateEquipActor);
}

void UPlAttchActorManage::SetPet(const FPetRow& pet_row)
{
	UnEquipPet();
	
	FActorSpawnParameters Param;

	Param.bNoFail = true;
	
	FVector Loc = UMyLib::GetNavSys()->GetRandomReachablePointInRadius(GetWorld(),m_PlOwner->GetActorLocation(),400);

	m_Pet = GetWorld()->SpawnActor<APetPawn>(APetPawn::StaticClass(),Loc,FRotator(0),Param);

	m_Pet->SetPetEntity(pet_row);
}

void UPlAttchActorManage::UnEquipPet()
{
	if(m_Pet)
	{
		m_Pet->Destroy();
	}
}

void UPlAttchActorManage::UpdateEquipActor()
{
	const TArray<FName>& AryEquips = UMyGameInstance::Get->m_EquipManager->GetEquipAry();

	int i=0;

	while(i<m_AryEqupActors.Num())
	{
		AAttachEquipmentBase* CurrentEquip = m_AryEqupActors[i];

		const FName& NewEquip = AryEquips[i];
		
		if(!NewEquip.IsNone())
		{
			if(CurrentEquip)
			{
				(CurrentEquip)->Destroy();
			}
			TSubclassOf<AAttachEquipmentBase> EquipClass = UMyLib::GetAttachItemClass(NewEquip);

			if(!EquipClass->IsValidLowLevel())
			{
				SpawnEquipActor(i,m_ClassEquip);
				return;
			}
			SpawnEquipActor(i,EquipClass);
		}
		else
		{
			if (i + 1 == (int)EEquipSlotType::Weapon)
			{
				SpawnEquipActor(i,m_ClassEquip);
			}
			else
			{
				if(CurrentEquip)
				{
					(CurrentEquip)->Destroy();
				}
				(CurrentEquip) = nullptr;
			}
		}
		i++;
	}
}

void UPlAttchActorManage::SpawnEquipActor(int indexSlot, TSubclassOf<AAttachEquipmentBase> classEquipActor)
{
	if(m_AryEqupActors[indexSlot])
	{
		if(m_AryEqupActors[indexSlot]->GetClass() == classEquipActor)
		{
			return;
		}
		m_AryEqupActors[indexSlot]->Destroy();
	}
	
	FActorSpawnParameters Params;
	
	Params.bNoFail = true;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AAttachEquipmentBase* SpawnActor = GetWorld()->SpawnActor<AAttachEquipmentBase>(classEquipActor,FVector(0),FRotator(0),Params);

	FName SocketName = SpawnActor->GetSocketName();

	m_AryEqupActors[indexSlot] = SpawnActor;

	SpawnActor->AttachToComponent(m_PlOwner->GetSkMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false),SocketName);
}