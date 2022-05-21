// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetReceiptEle.h"

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
}
