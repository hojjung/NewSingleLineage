// Fill out your copyright notice in the Description page of Project Settings.


#include "EE_Belt02.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UEE_Belt02::Equip()
{
	Super::Equip();
	
	UMyGameInstance::Get->m_EquipManager->EquipBelt(4);
}

void UEE_Belt02::UnEquip()
{
	Super::UnEquip();

	UMyGameInstance::Get->m_EquipManager->UnequipBelt();
}
