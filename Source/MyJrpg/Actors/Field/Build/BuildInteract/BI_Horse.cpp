// Fill out your copyright notice in the Description page of Project Settings.


#include "BI_Horse.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UBI_Horse::Init(const TArray<FString>& variable, UInventory* inven)
{
	Super::Init(variable, inven);
	
	if(inven)
	{
		m_Inven	 = inven;
	}
	else
	{
		m_Inven = NewObject<UInventory>(UMyGameInstance::Get);
		
		m_Inven->Init(1,NSLOCTEXT("UBI_Horse","HorseHouse","마구간"));
	}
	m_Player = UMyLib::GetPlayer();
}

bool UBI_Horse::IsEraseable()
{
	return m_Inven->GetUsingSlotCount() <= 0;
}

void UBI_Horse::OnInteract()
{
	UMyLib::GetCanvas()->OpenRidePanel(m_Inven);
}

UInventory* UBI_Horse::GetItemHolder()
{
	return m_Inven;
}
