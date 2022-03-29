// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetCollecPanelChild.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"



void UWidgetCollecPanelChild::Init(const FName& collecID, const FItemCollecRow& row)
{
	m_CollectionID = collecID;
	
	m_TextName->SetText(row.m_TextCollecName);

	SetOptionText(row.m_ClassOption);
	
	int Iter = 0;
	
	for(const FItemCollecNeed& ItemNeed : row.m_AryItems)
	{
		UWidgetCollecItemEle* ItemEle = CreateWidget<UWidgetCollecItemEle>(this, m_ClassItemEle);

		bool IsEquip = UMyLib::GetItemType( ItemNeed.m_Item.RowName) == EItemType::Equip;

		ItemEle->Init(m_CollectionID, Iter, IsEquip, ItemNeed.m_Item, ItemNeed.m_nEnchantLv);

		m_HoriElements->AddChild(ItemEle);

		Iter++;
	}
}

void UWidgetCollecPanelChild::SetOptionText(TSubclassOf<UOptionBase> classOp)
{
	UOptionBase* Op = classOp->GetDefaultObject<UOptionBase>();

	FStringFormatOrderedArguments ArgsOptionName;
	
	FStringFormatOrderedArguments ArgsValue;
	
	ArgsValue.Add(Op->GetEnchantValue(1));

	FString OptionText = FString::Format(*Op->GetOptionFormat(),ArgsValue);

	ArgsOptionName.Add(Op->GetOptionName());
	
	ArgsOptionName.Add(OptionText);

	FString TotalText = FString::Format(TEXT("{0}{1}"),ArgsOptionName);

	m_TextStat->SetText(FText::FromString(TotalText));
}

void UWidgetCollecPanelChild::Update()//사용자가 지금 가진거랑 비교해야함
{
	for(UWidget* ChildWidget : m_HoriElements->GetAllChildren())
	{
		UWidgetCollecItemEle* ItemEle = Cast<UWidgetCollecItemEle>(ChildWidget);

		ItemEle->Update();
	}
	
	FText ProgressText = UMyGameInstance::Get->m_ItemCollecManager->GetCollecProgressText(m_CollectionID);
	
	m_TextProgress->SetText(ProgressText);
}

TArray<UWidget*> UWidgetCollecPanelChild::GetChildElements()
{
	return m_HoriElements->GetAllChildren();
}

