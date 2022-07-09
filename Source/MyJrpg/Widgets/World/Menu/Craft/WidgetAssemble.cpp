// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetAssemble.h"

void UWidgetAssemble::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_ArySlots.Reset(10);
	m_ArySlots.Add(m_Slot01);
	m_ArySlots.Add(m_Slot02);
	m_ArySlots.Add(m_Slot03);
	m_ArySlots.Add(m_Slot04);
	m_ArySlots.Add(m_Slot05);
	m_ArySlots.Add(m_Slot06);
	m_ArySlots.Add(m_Slot07);
	m_ArySlots.Add(m_Slot08);
	m_ArySlots.Add(m_Slot09);
	m_ArySlots.Add(m_Slot10);
	
	int Index = 0;
	for(UWidgetCraftCostElement* Ele : m_ArySlots)
	{
		Ele->m_nIndex = Index;
		Ele->SetVisibility(ESlateVisibility::Collapsed);
		Ele->m_GetStackFuncPtr.BindUObject(this, &UWidgetAssemble::GetStack);
		Index++;
	}

	m_BtnPutAll->OnClicked.AddDynamic(this, &UWidgetAssemble::OnPutAll);

	m_BtnComplete->OnClicked.AddDynamic(this, &UWidgetAssemble::OnComplete);
}

void UWidgetAssemble::UpdateSlots()
{
	int Index = 0;
	for(const FItemSpec& ItemIn : m_AssembleInst.Get()->GetAryItems())
	{
		m_ArySlots[Index]->SetCraftCost(m_AssembleInst.Get()->GetCraftData().m_AryCostItem[Index]);
		Index++;
	}
}

int UWidgetAssemble::GetStack(int index)
{
	return m_AssembleInst->GetItemConstRef(index).m_nLvStack;
}

void UWidgetAssemble::ShowAssemble(UAssembleInst* assemble_inst)
{
	m_AssembleInst = assemble_inst;

	m_TextName->SetText(m_AssembleInst->GetInvenName());

	m_ImgIcon->SetBrushFromSoftTexture(assemble_inst->GetCraftData().m_Icon);

	int Index = 0;
	for(const FCraftItemCost& Cost : m_AssembleInst.Get()->GetCraftData().m_AryCostItem)
	{
		m_ArySlots[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Index++;
	}
	m_DeleHandle = m_AssembleInst->m_OnInvenChanged.AddUObject(this, &UWidgetAssemble::UpdateSlots);
	UpdateSlots();
}

void UWidgetAssemble::OnPutAll()
{
	//드래그 드랍으로 넣는것 먼저
	// int Index = 0;
	// for(const FCraftItemCost& Cost : m_AssembleInst.Get()->GetCraftData().m_AryCostItem)
	// {
	// 	if(!m_AssembleInst->IsSlotPutable(Index))
	// 	{
	// 		Index++;
	// 		continue;
	// 	}
	// 	m_AssembleInst->AddSlot(Index, );
	// 	m_AssembleInst->AddItemKey(Index, );
	//
	// 	m_AssembleInst->OnDropItem()
	// 	
	// 	Index++;
	// }
}

void UWidgetAssemble::OnComplete()
{
	
}

void UWidgetAssemble::ClosePanel()
{
	m_AssembleInst->m_OnInvenChanged.Remove(m_DeleHandle);
}
