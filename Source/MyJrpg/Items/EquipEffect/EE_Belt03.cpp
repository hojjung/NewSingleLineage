// Fill out your copyright notice in the Description page of Project Settings.


#include "EE_Belt03.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UEE_Belt03::Equip()
{
	Super::Equip();
	
	UMyGameInstance::Get->m_EquipManager->EquipBelt(5);
}

void UEE_Belt03::UnEquip()
{
	Super::UnEquip();

	UMyGameInstance::Get->m_EquipManager->UnequipBelt();
}
