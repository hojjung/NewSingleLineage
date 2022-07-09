// Fill out your copyright notice in the Description page of Project Settings.


#include "BI_ItemConverter.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UBI_ItemConverter::Init(const TArray<FString>& variable, UInventory* inven)
{
	Super::Init(variable, inven);

	if(inven)
	{
		m_ItemConvert = Cast<UItemConvertInst>(inven);	
	}
	else
	{
		m_ItemConvert = NewObject<UItemConvertInst>(UMyGameInstance::Get);
		
		const FItemConvertRow* ItemRow = UItemConvertTable::GetItemConverter->FindRow<FItemConvertRow>(*variable[0], "");
		
		m_ItemConvert->SetConvertData(*ItemRow);
	}
	UMyGameInstance::Get->m_ItemConvertManager->AddStructureAndItem(Cast<AStructureActor>(GetOuter()),m_ItemConvert);

}

bool UBI_ItemConverter::IsEraseable()
{
	return m_ItemConvert->IsEmpty();
}

void UBI_ItemConverter::OnInteract()
{
	UMyLib::GetCanvas()->OpenItemConverter(m_ItemConvert);
}

UInventory* UBI_ItemConverter::GetItemHolder()
{
	return m_ItemConvert;
}

void UBI_ItemConverter::OnStructureDestory()
{
	UMyGameInstance::Get->m_ItemConvertManager->RemoveStructureAndItem(Cast<AStructureActor>(GetOuter()));
}
