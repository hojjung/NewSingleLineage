#include "PetManager.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Preview/PreviewActor.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UPetManager::Init()
{
	UPetTable::GetPetTable->GetAllRows<FPetRow>("",m_AryPets);
}

const TArray<FPetRow*>& UPetManager::GetPetDatas() const
{
	return m_AryPets;
}

void UPetManager::CreatePreviewActor()
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;
	
	m_PreviewActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass(),FVector(9999,9999,9999),FRotator(0),Param);
}

void UPetManager::EquipPet(const FName& id)
{
	m_CrntPet = UPetTable::GetPetTable->FindRow<FPetRow>(id, "");
	
	EquipPet(*m_CrntPet);
}

void UPetManager::EquipPet(const FPetRow& selected)
{
	m_CrntPet = &selected;

	UMyLib::GetPlayer()->SetPet(*m_CrntPet);
	
	ShowPreviewPet(*m_CrntPet);

	m_OnPetChanged.Broadcast();
}

void UPetManager::UnEquip()
{
	m_CrntPet = nullptr;
	
	UMyLib::GetPlayer()->UnEquipPet();

	m_OnPetChanged.Broadcast();
}

void UPetManager::ShowPreviewPet(const FPetRow& selected)
{
	m_PreviewActor->OnMeshVisualChanged(selected);
}

void UPetManager::SetIsTouched(bool b)
{
	m_PreviewActor->SetIsTouched(b);
}

void UPetManager::RotatePawn(float delta_x)
{
	m_PreviewActor->RotatePawn(delta_x);
}

void UPetManager::ShowPawn()
{
	m_PreviewActor->ShowMeshWithTick();
}

void UPetManager::HidePawn()
{
	m_PreviewActor->HideMeshWithTick();
}

const FPetRow* UPetManager::GetCrntPet() const
{
	return m_CrntPet;
}
