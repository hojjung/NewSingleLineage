// Fill out your copyright notice in the Description page of Project Settings.


#include "Exe_EnchantArmor.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_EnchantArmor::Use(const FName& item,UInventory* inven) const
{
	UMyLib::GetCanvas()->OpenEnchant();
	//UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(item,inven);
}
