// Fill out your copyright notice in the Description page of Project Settings.


#include "BI_Storage.h"

#include "MyJrpg/MyLib.h"

void UBI_Storage::Init()
{
	Super::Init();
	m_Inven = NewObject<UInventory>(this);
	m_Inven->Init(20);
}

bool UBI_Storage::IsEraseable()
{
	return m_Inven->GetUsingSlotCount() <= 0;
}

void UBI_Storage::OnInteract()
{
	UMyLib::GetCanvas()->OpenStorage(m_Inven);
}
