// Fill out your copyright notice in the Description page of Project Settings.


#include "BI_Storage.h"

#include "MyJrpg/MyLib.h"

void UBI_Storage::Init(const FString& variable)
{
	Super::Init(variable);
	m_Inven = NewObject<UInventory>(this);
	int Size = FCString::Atoi(*variable);
	m_Inven->Init(Size);
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
