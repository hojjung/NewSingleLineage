#include "WidgetItemElement.h"
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
	}

	EItemType Type = UMyLib::GetItemType(ItemData);

	if(Type == EItemType::Equip)
	{
		if(UMyLib::GetEquip()->IsItemEquipped(ItemID))
		{
			return NSLOCTEXT("UWidgetItemElement","FocusUnequip","해제?");
		}
		else
		{
			return NSLOCTEXT("UWidgetItemElement","FocusEquip","장착?");
		}
	}
	else if(Type == EItemType::Consume)
	{
		return NSLOCTEXT("UWidgetItemElement","FocusUse","사용?");	
	}

	return FText();
}

void UWidgetItemElement::OnHoldingComplete()
{
	UMyLib::GetCanvas()->OpenItemInfo(GetItemID());

	SetMyUnFocus();
}

void UWidgetItemElement::SetIndex(int index)
{
	m_nIndex = index;
	UpdateElement();
}

FName UWidgetItemElement::GetItemID() const
{
	return m_Inven->GetItemID(m_nIndex);
}

void UWidgetItemElement::MoveItem(UInventory* addHere,UInventory* removeHere,const FName& ItemSpec, bool IsEquipItem)
{
	if(IsEquipItem)
	{
		int Level = m_Inven->GetItemLevel(ItemSpec);
		
		if(UMyLib::GetEquip()->IsItemEquipped(ItemSpec))
		{
			UMyGameInstance::Get->m_EquipManager->Unequip(UMyLib::GetItemData(ItemSpec).m_ItemType);
		}
		
		if(addHere->AddEquipItem(ItemSpec, Level))
		{
			removeHere->RemoveEquipItem(ItemSpec);
		}
	}
	else
	{
		int Amount = removeHere->GetItemStack(ItemSpec);
		
		if(addHere->AddItem(ItemSpec,Amount))
		{
			removeHere->RemoveItem(ItemSpec,Amount);
		}
	}
}


void UWidgetItemElement::SellItem()
{
	PRINTF("UUWidgetShopItemElement::TryBuyItem");

	const FName ID = m_Inven->GetItemID(m_nIndex);

	bool IsEquip =  UMyLib::GetItemType(ID) == EItemType::Equip;

	if(IsEquip)
	{
		m_nAmount = 1;
		
		m_nMaxAmount = 1;
		
		UMyGameInstance::Get->m_ShopManager->SellItem(ID,m_nAmount);

		return;
	}

	m_nAmount=0;

	int Gold = UMyGameInstance::Get->m_CurrencyManager->GetGold();

	m_nMaxAmount = m_Inven->GetItemStack(ID);

	UWidgetStackCalculator* Calculator = UMyLib::GetCanvas()->OpenCalculator(0);

	Calculator->m_OnNumberChanged.AddUObject(this,&UWidgetItemElement::OnSellConfirm);

	Calculator->m_OnGetMax.BindUObject(this,&UWidgetItemElement::GetMaxAmount);
}

void UWidgetItemElement::OnSellConfirm(int amount)
{
	m_nAmount = amount;

	if(m_nAmount<=0)
	{
		return;
	}
	
	UMyGameInstance::Get->m_ShopManager->SellItem(m_Inven->GetItemID(m_nIndex),m_nAmount);
	
	m_nAmount=0;	
}

int UWidgetItemElement::GetMaxAmount()
{
	return m_nMaxAmount;
}

void UWidgetItemElement::UseItem()
{
	FName ItemSpec = m_Inven->GetItemID(m_nIndex);

	EItemType ItemType = UMyLib::GetItemType(ItemSpec);

	bool IsEquipItem = ItemType == EItemType::Equip;

	switch (m_PanelType)
	{
	case EPanelType::StorageDeposit:
		MoveItem(UMyLib::GetPlayerStorage(),UMyLib::GetPlayerInven(),ItemSpec, IsEquipItem);
		return;
	case EPanelType::StorageWithdraw:
		MoveItem(UMyLib::GetPlayerInven(),UMyLib::GetPlayerStorage(),ItemSpec, IsEquipItem);
		return;
	case EPanelType::ShopSell:
		SellItem();
		return;
	case EPanelType::Enchant:
		if (IsEquipItem)
		{
			if (UMyGameInstance::Get->m_EnchantManager->IsAbleTarget(ItemSpec))
			{
				UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(ItemSpec);
			}
			return;
		}
	}
	
	if(IsEquipItem)
	{
		UMyGameInstance::Get->m_ItemExeManager->EquipUnequipItem(ItemSpec);
		
		return;
	}
	
	UMyGameInstance::Get->m_ItemExeManager->ExecuteItem(ItemSpec);
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

void UWidgetItemElement::UpdateElement(const FName& id)
{
	m_TextStackAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(id);

	if(UMyLib::GetItemType(ItemData) == EItemType::Equip)
	{
		int Level = m_Inven->GetItemLevel(id);

		if(Level>0)
		{
			FString LevelStr = FString::Printf(TEXT("+ %d"),Level);

			m_TextStackAmount->SetText(FText::FromString(LevelStr));
		}
		else
		{
			m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);
		}

		if(UMyLib::GetEquip()->IsItemEquipped(id))
		{
			m_OverlayEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			m_OverlayEquip->SetVisibility(ESlateVisibility::Collapsed);
		}	
	}
	else
	{
		int StackCount =m_Inven->GetItemStack(id);

		m_TextStackAmount->SetText(FText::AsNumber(StackCount));	
	}

	m_ElementBase->SetHoldable(true);

	m_ElementBase->SetIcon(ItemData.m_ItemIcon);

	m_ElementBase->SetGlowColor(ItemData.m_ColorHandle);
}

void UWidgetItemElement::OnClicked()
{
	if(IsSlotEmpty())
	{
		return;
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


