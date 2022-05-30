// Fill out your copyright notice in the Description page of Project Settings.


#include "BI_ItemConverter.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UBI_ItemConverter::Init(const FString& variable)
{
	Super::Init(variable);
	
	m_ItemConvert = NewObject<UItemConvertInst>(this);
	UMyGameInstance::Get->m_ItemConvertManager->AddStructureAndItem(Cast<AStructureActor>(GetOuter()),m_ItemConvert);

	const FItemConvertRow* ItemRow = UItemConvertTable::GetItemConverter->FindRow<FItemConvertRow>(*variable, "");
	
	m_ItemConvert->SetConvertData(*ItemRow);
}

bool UBI_ItemConverter::IsEraseable()
{
	return m_ItemConvert->IsEmpty();
}

void UBI_ItemConverter::OnInteract()
{
	UMyLib::GetCanvas()->OpenItemConverter(m_ItemConvert);
}
