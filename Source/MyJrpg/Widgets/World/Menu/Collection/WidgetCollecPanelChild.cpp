// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetCollecPanelChild.h"

#include "MyJrpg/MyLib.h"

void UWidgetCollecPanelChild::Init(const FName& collecID, const FItemCollecRow& row)
{
	int Iter = 0;
	
	for(const FItemCollecNeed& ItemNeed : row.m_AryItems)
	{
		UWidgetCollecItemEle* ItemEle = CreateWidget<UWidgetCollecItemEle>(this, m_ClassItemEle);

		bool IsEquip = UMyLib::GetItemType( ItemNeed.m_Item.RowName) == EItemType::Equip;

		ItemEle->Init(collecID, Iter, IsEquip, ItemNeed.m_Item, ItemNeed.m_nEnchantLv);

		m_HoriElements->AddChild(ItemEle);

		Iter++;
	}
}

void UWidgetCollecPanelChild::Update()//사용자가 지금 가진거랑 비교해야함
{
	for(UWidget* ChildWidget : m_HoriElements->GetAllChildren())
	{
		UWidgetCollecItemEle* ItemEle = Cast<UWidgetCollecItemEle>(ChildWidget);

		ItemEle->Update();
	}
}
