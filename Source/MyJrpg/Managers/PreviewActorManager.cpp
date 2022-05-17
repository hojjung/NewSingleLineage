// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewActorManager.h"

#include "MyJrpg/Actors/Preview/PreviewActor.h"

void UPreviewActorManager::CreatePreviewActor()
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;
	
	m_PreviewActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass(),FVector(9999,9999,9999),FRotator(0),Param);
	m_PreviewActor->SetupPlayerPreview();
}
//AnimSequence'/Game/14_ModularArmor/Anims/Spear/Frank_RPG_Spear_Unequip_Idle.Frank_RPG_Spear_Unequip_Idle'
void UPreviewActorManager::SetIsTouched(bool b)
{
	m_PreviewActor->SetIsTouched(b);
}

void UPreviewActorManager::RotatePawn(float delta_x)
{
	m_PreviewActor->RotatePawn(delta_x);
}

void UPreviewActorManager::ShowPawn()
{
	m_PreviewActor->ShowMeshWithTick();
}

void UPreviewActorManager::HidePawn()
{
	m_PreviewActor->HideMeshWithTick();
}

void UPreviewActorManager::Update(const AModularUnitPawn* pawn)
{
	m_PreviewActor->OnMeshVisualChanged(pawn);
}
