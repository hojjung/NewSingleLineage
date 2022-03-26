// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarManager.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Preview/PreviewActor.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UAvatarManager::CreatePreviewActor()
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;
	
	m_PreviewActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass(),FVector(9999,9999,9999),FRotator(0,0,0),Param);
}

void UAvatarManager::EquipSkin(const FName& id)
{
	const FPlayerUnitEntityRow* UnitRow = UUnitEntityData::GetPlayerUnitTable->FindRow<FPlayerUnitEntityRow>(id, "");
	
	UMyLib::GetPlayer()->SetPlayerEntity(*UnitRow);

	m_PreviewActor->OnMeshVisualChanged(*UnitRow);

	UMyLib::GetPlayerCon()->ClientForceGarbageCollection();
}

void UAvatarManager::SetIsTouched(bool b)
{
	m_PreviewActor->SetIsTouched(b);
}

void UAvatarManager::RotatePawn(float delta_x)
{
	m_PreviewActor->RotatePawn(delta_x);
}
