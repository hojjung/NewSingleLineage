// Fill out your copyright notice in the Description page of Project Settings.


#include "Exe_EnchantTrinket.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_EnchantTrinket::Use(const FName& item) const
{
	UMyLib::GetCanvas()->OpenEnchant();
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(item);
}
