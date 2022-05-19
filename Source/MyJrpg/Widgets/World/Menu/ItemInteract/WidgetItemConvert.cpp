#include "WidgetItemConvert.h"

#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"

void UWidgetItemConvert::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Inven->Init(UMyLib::GetPlayerInven());

	m_LeftItem->m_OnFocus.AddUObject(this,&UWidgetItemConvert::OnFocused);
	m_LeftItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_LeftItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);
	m_RightItem->m_OnFocus.AddUObject(this,&UWidgetItemConvert::OnFocused);
	m_RightItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_RightItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);
	m_CostItem->m_OnFocus.AddUObject(this,&UWidgetItemConvert::OnFocused);
	m_CostItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_CostItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);

	m_LeftItem->SetIndex(0);
	m_RightItem->SetIndex(1);
	m_CostItem->SetIndex(2);
	
	m_AryItems.Reset();
	m_AryItems.Add(m_LeftItem);
	m_AryItems.Add(m_RightItem);
	m_AryItems.Add(m_CostItem);
}

void UWidgetItemConvert::ClosePanel()
{
	Super::ClosePanel();
	m_ItemConvertInst->m_OnChanged.Remove(m_Dele);
	m_ItemConvertInst.Reset();
}

void UWidgetItemConvert::UpdatePanel()
{
	m_Inven->OpenPanel();
	
	// m_Bag->Init(UMyGameInstance::Get->m_EquipManager->GetBag());
	// m_Bag->OpenPanel();
	//
	// m_Belt->Init(UMyGameInstance::Get->m_EquipManager->GetBelt());
	// m_Belt->OpenPanel();
	//
	UpdateElement(m_LeftItem,m_ItemConvertInst->GetLeftItem());
	UpdateElement(m_RightItem,m_ItemConvertInst->GetRightItem());
	UpdateElement(m_CostItem,m_ItemConvertInst->GetCostItem());
}

void UWidgetItemConvert::UpdateElement(UWidgetBaseElement* ele, const FItemSpec& item)
{
	if(item.m_ID.IsNone())
	{
		ele->Clear();
		return ;
	}
	const FItemDataRow& Data = UMyLib::GetItemData(item.m_ID);

	ele->SetIcon(Data.m_Icon);
	
	ele->SetGlowColor(Data.m_ColorHandle);
}

void UWidgetItemConvert::OnFocused(UWidgetBaseElement* ele)
{
	
}

void UWidgetItemConvert::OnDrag(UWidgetBaseElement* ele)
{
	
}

void UWidgetItemConvert::OnDrop(UWidgetBaseElement* ele)
{
	// if(UItemDDO::GetDDOInst->m_FromInven.Get())
	// {
	// 	m_ItemConvertInst->OnDropItem(UItemDDO::GetDDOInst->m_FromInven.Get(),UItemDDO::GetDDOInst->m_nIndex);
	//
	// 	return;
	// }
	//
	// if(UItemDDO::GetDDOInst->m_FromEquip.Get())
	// {
	// 	FItemSpec MyItem =  ele->GetIndex()
	//
	// 	FItemSpec OtherItem = UItemDDO::GetDDOInst->GetItem();
	// 	
	// 	if(MyItem.m_ID.IsNone())
	// 	{
	// 		int InvenIndex = ele->GetIndex();
	// 		
	// 		UMyLib::GetEquip()->Unequip((EEquipSlotType)UItemDDO::GetDDOInst->m_nIndex,m_CurrentInven.Get(),&InvenIndex);
	// 		m_CurrentInven->UpdateInventory();
	// 		return;
	// 	}
	// 	
	//
	// 	FItemDataRow A = UMyLib::GetItemData(MyItem.m_ID);
	//
	// 	FItemDataRow B = UMyLib::GetItemData(OtherItem.m_ID);
	// 	
	// 	if(A.m_ItemType != B.m_ItemType)
	// 	{
	// 		return;
	// 	}
	// 	UMyLib::GetEquip()->Equip((EEquipSlotType)UItemDDO::GetDDOInst->m_nIndex,m_CurrentInven.Get(),ele->GetIndex());
	// 	m_CurrentInven->UpdateInventory();
	// 	return;
	// }
	UInventory* Inven = UItemDDO::GetDDOInst->m_FromInven.Get();
	
	int FromInvenIndex = UItemDDO::GetDDOInst->m_nIndex;
	
	int Index = ele->GetIndex();

	switch (Index)
	{
	case 0:
		m_ItemConvertInst->SetLeftItem(Inven,FromInvenIndex);
		break;
	case 2:
		m_ItemConvertInst->SetCostItem(Inven,FromInvenIndex);
		break;
	}
}

void UWidgetItemConvert::ShowItemConvert(UItemConvertInst* inst)
{
	OpenPanel();
	m_ItemConvertInst = inst;
	m_Dele = m_ItemConvertInst->m_OnChanged.AddUObject(this, &UWidgetItemConvert::UpdatePanel);
	UpdatePanel();
}
