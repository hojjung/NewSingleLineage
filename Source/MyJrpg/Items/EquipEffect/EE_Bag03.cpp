// Fill out your copyright notice in the Description page of Project Settings.


#include "EE_Bag03.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UEE_Bag03::Equip()
{
	Super::Equip();

	UMyGameInstance::Get->m_EquipManager->EquipBag(18);
}

void UEE_Bag03::UnEquip()
{
	Super::UnEquip();

	UMyGameInstance::Get->m_EquipManager->UnequipBag();
}
