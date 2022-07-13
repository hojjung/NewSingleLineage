// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetRidePanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"

void UWidgetRidePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_Inven->Init(UMyLib::GetPlayerInven());
	m_Inven->m_OnFocus.AddUObject(this, &UWidgetRidePanel::OnPlInvenFocused);

	m_CostItem->m_OnDrag.AddUObject(this,&UWidgetRidePanel::OnDrag);
	m_CostItem->m_OnDrop.AddUObject(this,&UWidgetRidePanel::OnDrop);
}

void UWidgetRidePanel::ShowRidePanel(UInventory* inst)
{
	OpenPanel();
	
	m_RideInven = inst;
	
	m_Dele = m_RideInven->m_OnInvenChanged.AddUObject(this, &UWidgetRidePanel::UpdatePanel);

	m_TextRideName->SetText(m_RideInven->GetInvenName());

	m_RiderCost->TryVisible();

	UpdatePanel();
}

void UWidgetRidePanel::UpdatePanel()
{
	m_Inven->OpenPanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_Bag->Init(UMyGameInstance::Get->m_EquipManager->GetBag());
		m_Bag->OpenPanel();
	}

	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_Belt->Init(UMyGameInstance::Get->m_EquipManager->GetBelt());
		m_Belt->OpenPanel();
	}
	
	UpdateElement(m_CostItem,m_RideInven->GetItemConstRef(0));
}

void UWidgetRidePanel::UpdateElement(UWidgetBaseElement* ele, const FItemSpec& item)
{
	if(item.m_ID.IsNone())
	{
		ele->Clear();
		UMyGameInstance::Get->m_ZoneMove->SetRiderEnergy(0);
		return ;
	}

	ele->SetItem(item);
	
	ele->SetDragable(true);

	UMyGameInstance::Get->m_ZoneMove->SetRiderEnergy(item.m_nLvStack * 5);
}
void UWidgetRidePanel::OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	ele->SetMyUnFocus();
}

void UWidgetRidePanel::OnDrag(UWidgetBaseElement* ele)
{
	UItemDDO::GetDDOInst->m_FromRideCost = m_RideInven;

	UItemDDO::GetDDOInst->m_nIndex = 0;
}

void UWidgetRidePanel::OnDrop(UWidgetBaseElement* ele)
{
	if(UItemDDO::GetDDOInst->m_FromInven.Get())
	{
		const FItemSpec& ItemSpec = UItemDDO::GetDDOInst->GetItem();
		
		if(ItemSpec.m_ID != TEXT("Carrot"))
		{
			return;
		}
		
		m_RideInven->OnDropItem(0,UItemDDO::GetDDOInst->m_FromInven.Get(),UItemDDO::GetDDOInst->m_nIndex);
	}
}
void UWidgetRidePanel::ClosePanel()
{
	Super::ClosePanel();
	m_RideInven->m_OnInvenChanged.Remove(m_Dele);
	m_RideInven.Reset();
}