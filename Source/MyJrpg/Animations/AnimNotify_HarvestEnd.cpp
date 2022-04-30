// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_HarvestEnd.h"

#include "MyJrpg/Actors/Field/Gather/TreeBase.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void UAnimNotify_HarvestEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	Super::Notify(MeshComp, Animation);
	
	if(!MeshComp->GetOwner())
	{
		return;
	}

	ACombatUnitPawn* Pawn =  Cast<ACombatUnitPawn>( MeshComp->GetOwner());

	if(!Pawn)
	{
		return;
	}

	ATreeBase* Tree =  Pawn->GetFocusedTarget<ATreeBase>();

	if(!Tree)
	{
		return;
	}

	Tree->OnHarvestMotionDone();
}
