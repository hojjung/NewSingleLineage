#include "AvatarManager.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Preview/PreviewActor.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UAvatarManager::Init()
{
	UUnitEntityData::GetPlayerUnitTable->GetAllRows<FPlayerUnitEntityRow>("",m_ArySkins);

	m_CrntSkin = UUnitEntityData::GetPlayerUnitTable->FindRow<FPlayerUnitEntityRow>(TEXT("Player01"), "");
}

const TArray<FPlayerUnitEntityRow*>& UAvatarManager::GetAllSkins() const
{
	return m_ArySkins;
}

void UAvatarManager::CreatePreviewActor()
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;
	
	m_PreviewActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass(),FVector(9999,9999,9999),FRotator(0),Param);
}

void UAvatarManager::EquipSkin(const FName& id)
{
	m_CrntSkin = UUnitEntityData::GetPlayerUnitTable->FindRow<FPlayerUnitEntityRow>(id, "");
	
	EquipSkin(*m_CrntSkin);
}

void UAvatarManager::EquipSkin(const FPlayerUnitEntityRow& selected)
{
	m_CrntSkin = &selected;

	ShowPreviewSkin(*m_CrntSkin);
	
	UMyLib::GetPlayer()->SetPlayerEntity(*m_CrntSkin);

	m_OnSkinChanged.Broadcast();
}

void UAvatarManager::ShowPreviewSkin(const FPlayerUnitEntityRow& selected)
{
	m_PreviewActor->OnMeshVisualChanged(selected);
}

void UAvatarManager::SetIsTouched(bool b)
{
	m_PreviewActor->SetIsTouched(b);
}

void UAvatarManager::RotatePawn(float delta_x)
{
	m_PreviewActor->RotatePawn(delta_x);
}

void UAvatarManager::ShowPawn()
{
	m_PreviewActor->ShowMeshWithTick();
}

void UAvatarManager::HidePawn()
{
	m_PreviewActor->HideMeshWithTick();
}

const FPlayerUnitEntityRow* UAvatarManager::GetCrntSkin() const
{
	return m_CrntSkin;
}
