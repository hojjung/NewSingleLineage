// Fill out your copyright notice in the Description page of Project Settings.


#include "Exe_EnchantArmor.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_EnchantArmor::Use(FItemSpec& item) const
{
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(item);
	UMyLib::GetCanvas()->OpenEnchant();
}
