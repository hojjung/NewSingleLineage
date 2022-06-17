// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetReceiptEle.h"

#include "MyJrpg/MyLib.h"

void UWidgetReceiptEle::SetElement(const FItemConvertSet& item_converts)
{
	if(item_converts.m_CostItem.RowName.IsNone())
	{
		m_CostItem->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		int CostSt = item_converts.m_nCostItemStLv;

		m_CostItem->SetItem(FItemSpec(item_converts.m_CostItem.RowName,CostSt));
	}
	
	int LeftSt = item_converts.m_nLeftItemStLv;
	
	int RightSt = item_converts.m_nRightItemStLv;
	
	m_LeftItem->SetItem(FItemSpec(item_converts.m_LeftItem.RowName,LeftSt));
	
	m_RightItem->SetItem(FItemSpec(item_converts.m_RightItem.RowName,RightSt));

	m_LeftItem->SetHoldable(true);
	m_LeftItem->SetDragable(false);
	m_LeftItem->SetFocusable(false);

	m_RightItem->SetHoldable(true);
	m_RightItem->SetDragable(false);
	m_RightItem->SetFocusable(false);

	m_LeftItem->m_OnHold.AddUObject(this,&UWidgetReceiptEle::OnHold);

	m_RightItem->m_OnHold.AddUObject(this,&UWidgetReceiptEle::OnHold);

	m_CostItem->m_OnHold.AddUObject(this,&UWidgetReceiptEle::OnHold);
}


void UWidgetReceiptEle::OnHold(UWidgetBaseElement* ele)
{
	UMyLib::GetCanvas()->OpenItemInfoData(*ele->GetItemData());
}