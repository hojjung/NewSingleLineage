// Fill out your copyright notice in the Description page of Project Settings.


#include "EE_Bag02.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UEE_Bag02::Equip()
{
	Super::Equip();

	UMyGameInstance::Get->m_EquipManager->EquipBag(12);
}

void UEE_Bag02::UnEquip()
{
	Super::UnEquip();
	UMyGameInstance::Get->m_EquipManager->UnequipBag();
}
