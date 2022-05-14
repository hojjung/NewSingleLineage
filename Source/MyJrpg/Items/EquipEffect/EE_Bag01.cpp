// Fill out your copyright notice in the Description page of Project Settings.


#include "EE_Bag01.h"

#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UEE_Bag01::Equip()
{
	Super::Equip();

	UMyGameInstance::Get->m_EquipManager->EquipBag(14);
}

void UEE_Bag01::UnEquip()
{
	Super::UnEquip();
	UMyGameInstance::Get->m_EquipManager->UnequipBag();
}
