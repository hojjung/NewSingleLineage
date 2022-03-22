// Fill out your copyright notice in the Description page of Project Settings.


#include "Exe_EnchantTrinket.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_EnchantTrinket::Use(FItemSpec& item) const
{
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(item);
	UMyLib::GetCanvas()->OpenEnchant();
}
