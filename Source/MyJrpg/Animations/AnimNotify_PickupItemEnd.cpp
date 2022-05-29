// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PickupItemEnd.h"

#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void UAnimNotify_PickupItemEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
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

	AItemActor* ItemActor =  Pawn->GetFocusedTarget<AItemActor>();

	if(!ItemActor)
	{
		return;
	}

	ItemActor->OnObtain();
}
