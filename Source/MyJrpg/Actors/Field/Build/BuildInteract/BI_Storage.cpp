// Fill out your copyright notice in the Description page of Project Settings.


#include "BI_Storage.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UBI_Storage::Init(const TArray<FString>& variable, UInventory* inven)
{
	Super::Init(variable, inven);
	
	if(inven)
	{
		m_Inven	 = inven;
	}
	else
	{
		m_Inven = NewObject<UInventory>(UMyGameInstance::Get);
		
		int Size = FCString::Atoi(*variable[0]);
		
		m_Inven->Init(Size,NSLOCTEXT("UBI_Storage","StorageName","창고"));
	}
	m_Player = UMyLib::GetPlayer();
}

bool UBI_Storage::IsEraseable()
{
	return m_Inven->GetUsingSlotCount() <= 0;
}

void UBI_Storage::OnInteract()
{
	UMyLib::GetCanvas()->OpenStorage(m_Inven);
}

UInventory* UBI_Storage::GetItemHolder()
{
	return m_Inven;
}
