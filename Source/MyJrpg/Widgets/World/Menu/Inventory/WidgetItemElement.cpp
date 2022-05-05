#include "WidgetItemElement.h"

#include "ItemDDO.h"
#include "WidgetInventory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetItemElement::Init(EPanelType panel, UInventory* inven)
{
	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetItemElement::OnHoldingComplete);

	m_ElementBase->m_OnClick.AddUObject(this,&UWidgetItemElement::OnClicked);

	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
	
	m_PanelType = panel;

	if (m_PanelType == EPanelType::None)
	{
		m_ElementBase->SetHoldable(false);
		m_ElementBase->SetFocusable(false);
	}
	else
	{
		m_ElementBase->SetHoldable(true);
		m_ElementBase->SetFocusable(true);
	}

	m_Inven = inven;

	m_nIndex = INDEX_NONE;

	Clear();
}

void UWidgetItemElement::Clear()
{
	m_ElementBase->Clear();

	m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);

	m_OverlayEquip->SetVisibility(ESlateVisibility::Collapsed);
}

FText UWidgetItemElement::GetFocusText()
{
	FName ItemID = GetItemID();
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(ItemID);

	switch (m_PanelType)
	{
	case EPanelType::StorageDeposit:
		return NSLOCTEXT("UWidgetItemElement","FocusDeposit","넣기?");
	case EPanelType::StorageWithdraw:
		return NSLOCTEXT("UWidgetItemElement","FocusWithdraw","꺼내기?");
	case EPanelType::ShopBuy:
		return NSLOCTEXT("UWidgetItemElement","FocusBuy","사기?");
	case EPanelType::ShopSell:
		return NSLOCTEXT("UWidgetItemElement","FocusSell","팔기?");
	case EPanelType::Enchant:
		return NSLOCTEXT("UWidgetItemElement","Enchant","선택?");
	case EPanelType::PickPocketPl:
		return NSLOCTEXT("UWidgetItemElement","PickPocketPl","선물?");
	case EPanelType::PickPocketTarget:
		return GetPickPocketText();
	}

	EItemType Type = UMyLib::GetItemType(ItemData);

	if(Type == EItemType::Equip)
	{
		// if(UMyLib::GetEquip()->IsItemEquipped(ItemID))
		// {
		// 	return NSLOCTEXT("UWidgetItemElement","FocusUnequip","해제?");
		// }
		// else
		// {
		// 	return NSLOCTEXT("UWidgetItemElement","FocusEquip","장착?");
		// }
	}
	else if(Type == EItemType::Consume)
	{
		return NSLOCTEXT("UWidgetItemElement","FocusUse","사용?");	
	}

	return FText();
}

void UWidgetItemElement::OnHoldingComplete()
{
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Inven,GetItemID(),m_Inven.Get());

	SetMyUnFocus();
}

void UWidgetItemElement::SetIndex(int index)
{
	m_nIndex = index;
	UpdateElement();
}

const FName& UWidgetItemElement::GetItemID() const
{
	return m_Inven->GetAryItems()[m_nIndex].m_ID;
}

void UWidgetItemElement::MoveItem(UInventory* addHere,UInventory* removeHere, int targetIndexFromRemoveHere)
{
	const FItemSpec& Item = removeHere->GetItem(targetIndexFromRemoveHere);
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(Item.m_ID);
	
	bool IsEquip = UMyLib::IsEquip(ItemData);
	
	if(IsEquip)
	{
		if(UMyLib::GetEquip()->IsItemEquipped(Item))
		{
			UMyGameInstance::Get->m_EquipManager->Unequip(ItemData.m_ItemType);
		}
	}
	if (addHere->AddItem(Item))
	{
		removeHere->RemoveItem(targetIndexFromRemoveHere);
	}
}

void UWidgetItemElement::TryPickPocketItem(UInventory* addHere, AMonsterPawn* target, const FName& ItemSpec, bool IsEquipItem)
{
	// float PickPocketRate = 0.52f;
	//
	// float Rand = FMath::RandRange(0,1);
	//
	// if(Rand <= PickPocketRate)
	// {
	// 	MoveItem(addHere,target->GetInven(),ItemSpec,IsEquipItem);
	//
	// 	return;
	// }
	//
	// UMyGameInstance::Get->m_TeamKarma->DecreaseKarma(target->GetTeamID(),60);
	//
	// UWidgetPickpocketPanel* Panel = UMyLib::GetCanvas()->GetPickpocketMenu();
	//
	// Panel->GetCurrentTargetPawn()->SetFocusedTarget(UMyLib::GetPlayer());
	//
	// Panel->ClosePanel();
}

void UWidgetItemElement::SellItem()
{
	// const FName ID = m_Inven->GetItemID(m_nIndex);
	//
	// bool IsEquip =  UMyLib::GetItemType(ID) == EItemType::Equip;
	//
	// if(IsEquip || m_Inven->GetItemStack(ID) == 1)
	// {
	// 	m_nAmount = 1;
	// 	
	// 	m_nMaxAmount = 1;
	// 	
	// 	UMyGameInstance::Get->m_ShopManager->SellItem(ID,m_nAmount);
	//
	// 	return;
	// }
	//
	// m_nAmount=0;
	//
	// m_nMaxAmount = m_Inven->GetItemStack(ID);
	//
	// UWidgetStackCalculator* Calculator = UMyLib::GetCanvas()->OpenCalculator(0);
	//
	// Calculator->m_OnNumberAccept.AddUObject(this,&UWidgetItemElement::OnSellConfirm);
	//
	// Calculator->m_OnGetMax.BindUObject(this,&UWidgetItemElement::GetMaxAmount);
}

void UWidgetItemElement::OnSellConfirm(int amount)
{
	m_nAmount = amount;

	if(m_nAmount<=0)
	{
		return;
	}
	
	UMyGameInstance::Get->m_ShopManager->SellItem(GetItemID(),m_nAmount);
	
	m_nAmount=0;	
}

int UWidgetItemElement::GetMaxAmount()
{
	return m_nMaxAmount;
}

void UWidgetItemElement::UseItem()
{
	const FName& ItemSpec = GetItemID();

	EItemType ItemType = UMyLib::GetItemType(ItemSpec);

	bool IsEquipItem = ItemType == EItemType::Equip;

	switch (m_PanelType)
	{
	case EPanelType::StorageDeposit:
		MoveItem(UMyLib::GetFocusedStroage(),UMyLib::GetPlayerInven(),m_nIndex);
		return;
	case EPanelType::StorageWithdraw:
		MoveItem(UMyLib::GetPlayerInven(),UMyLib::GetFocusedStroage(),m_nIndex);
		return;
	case EPanelType::PickPocketPl:
		//MoveItem(UMyLib::GetPickPocketTarget()->GetInven(), UMyLib::GetPlayerInven(), ItemSpec, IsEquipItem);
		return;
	case EPanelType::PickPocketTarget:
		TryPickPocketItem(UMyLib::GetPlayerInven(),UMyLib::GetPickPocketTarget(),ItemSpec, IsEquipItem);
		return;
	case EPanelType::ShopSell:
		SellItem();
		return;
	case EPanelType::Enchant:
		if (IsEquipItem)
		{
			if (UMyGameInstance::Get->m_EnchantManager->IsAbleTarget(ItemSpec))
			{
				//UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(ItemSpec,m_Inven.Get());
			}
			return;
		}
	}
	
	if(IsEquipItem)
	{
		UMyGameInstance::Get->m_ItemExeManager->EquipUnequipItem(ItemSpec);
		
		return;
	}
	
	UMyGameInstance::Get->m_ItemExeManager->ExecuteItem(ItemSpec,m_Inven.Get());
}

void UWidgetItemElement::UpdateElement()
{
	Clear();

	if(IsSlotEmpty())
	{
		return;
	}

	UpdateElement(GetItemID());
}

FText UWidgetItemElement::GetPickPocketText()
{
	FText T = NSLOCTEXT("UWidgetItemElement","PickPocketTarget","훔치기:");

	float PickPocketRate = 0.52f;

	FString StrF = FString::Printf(TEXT("%s%.0f%%"),*T.ToString(), PickPocketRate * 100.f);

	return FText::FromString(StrF);
}

void UWidgetItemElement::UpdateElement(const FName& id)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(id);

	int Level = m_Inven.Get() ? m_Inven->GetAryItems()[m_nIndex].m_nLvStack : 0;

	if(UMyLib::GetItemType(ItemData) == EItemType::Equip)
	{
		if(Level > 0)
		{
			m_TextStackAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			m_TextStackAmount->SetText(FText::FromString(FString::Printf(TEXT("+ %d"),Level)));
		}
		else
		{
			m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);
		}

		// if(UMyLib::GetEquip()->IsItemEquipped(id))
		// {
		// 	m_OverlayEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		// }
		// else
		// {
		// 	m_OverlayEquip->SetVisibility(ESlateVisibility::Collapsed);
		// }	
	}
	else
	{
		if (Level > 0)
		{
			m_TextStackAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			m_TextStackAmount->SetText(FText::AsNumber(Level));	
		}
		else
		{
			m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	m_ElementBase->SetHoldable(true);

	m_ElementBase->SetIcon(ItemData.m_Icon);

	m_ElementBase->SetGlowColor(ItemData.m_ColorHandle);
}

void UWidgetItemElement::OnClicked()
{
	if(IsSlotEmpty())
	{
		return;
	}

	if (m_PanelType == EPanelType::Inven)
	{
		EItemType ItemType = UMyLib::GetItemType(GetItemID());

		if (ItemType == EItemType::None || ItemType == EItemType::misc)
		{
			return;
		}
	}

	if(m_TextFocus->IsVisible())
	{
		UseItem();
		
		SetMyUnFocus();
		
		return;
	}

	m_OnFocus.Broadcast(this);//다른애 풀림

	m_TextFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextFocus->SetText(GetFocusText());

	m_ElementBase->SetMyFocus();
}

void UWidgetItemElement::SetMyUnFocus()
{
	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ElementBase->SetMyUnFocus();
}

bool UWidgetItemElement::IsSlotEmpty()
{
	return  GetIndex() == INDEX_NONE|| GetItemID().IsNone();
}

int UWidgetItemElement::GetIndex()
{
	return m_nIndex;
}

void UWidgetItemElement::SetMyInteractable(bool isInteractAble)
{
	m_ElementBase->SetHoldable(isInteractAble);
}