#include "WidgetItemElement.h"
#include "WidgetInventory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetItemElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nIndex = INDEX_NONE;

	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetItemElement::OnHoldingComplete);

	m_ElementBase->m_OnClick.AddUObject(this,&UWidgetItemElement::OnClicked);

	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);

	m_PanelType = EPanelType::Inven;
}

void UWidgetItemElement::Clear()
{
	m_ElementBase->Clear();

	m_ElementBase->SetHoldable(false);

	m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);

	m_OverlayEquip->SetVisibility(ESlateVisibility::Collapsed);
}

FText UWidgetItemElement::GetFocusText()
{
	const FItemSpec& ItemSpec = *GetItemSpec();
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(ItemSpec);

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
		if(UMyLib::GetEquip()->IsItemEquipped(ItemSpec))
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
	UMyLib::GetCanvas()->OpenItemInfo(*GetItemSpec());

	SetMyUnFocus();
}

const FItemSpec* UWidgetItemElement::GetItemSpec() const
{
	return &m_Inven->GetItem(m_nIndex);
}

void UWidgetItemElement::MoveItem(UInventory* addHere,UInventory* removeHere,const FItemSpec& ItemSpec, bool IsEquipItem)
{
	int Amount = ItemSpec.m_nStack;
	
	UInventory::EResult Result;
	
	if(IsEquipItem)
	{
		Result = addHere->AddItemInst(ItemSpec);

		if(UMyLib::GetEquip()->IsItemEquipped(ItemSpec))
		{
			UMyGameInstance::Get->m_EquipManager->Unequip(UMyLib::GetItemData(ItemSpec).m_ItemType);
		}
	}
	else
	{
		Result = addHere->AddItem(ItemSpec.m_ItemID,Amount,ItemSpec.m_nLevel);
	}
		
	if(Result == UInventory::EResult::Success)
	{
		removeHere->RemoveItem(ItemSpec);
	}
	else if(Result == UInventory::EResult::FailStackCount)
	{
		int Diff = ItemSpec.m_nStack - Amount;

		removeHere->RemoveItem(ItemSpec.m_ItemID,Diff);
	}
	else
	{
		//칸수 부족 그냥 무시
	}
}


void UWidgetItemElement::SellItem()
{
	PRINTF("UUWidgetShopItemElement::TryBuyItem");

	const FItemSpec& ItemSpec = m_Inven->GetItem(m_nIndex);

	bool IsEquip =  UMyLib::GetItemType(ItemSpec.m_ItemID) == EItemType::Equip;

	if(IsEquip)
	{
		m_nAmount = 1;
		
		m_nMaxAmount = 1;
		
		UMyGameInstance::Get->m_ShopManager->SellItem(ItemSpec,m_nAmount);

		return;
	}

	m_nAmount=0;

	int Gold = UMyGameInstance::Get->m_CurrencyManager->GetGold();

	m_nMaxAmount = ItemSpec.m_nStack;

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
	
	const FItemSpec& ItemSpec = m_Inven->GetItem(m_nIndex);

	UMyGameInstance::Get->m_ShopManager->SellItem(ItemSpec,m_nAmount);
	
	m_nAmount=0;	
}

int UWidgetItemElement::GetMaxAmount()
{
	return m_nMaxAmount;
}

void UWidgetItemElement::UseItem()
{
	FItemSpec& ItemSpec = m_Inven->GetItem(m_nIndex);

	EItemType ItemType = UMyLib::GetItemType(ItemSpec.m_ItemID);

	bool IsEquipItem = ItemType == EItemType::Equip;

	UInventory::EResult Result = UInventory::EResult::Success;

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

void UWidgetItemElement::SetIndex(int index)
{
	m_nIndex = index;
}

void UWidgetItemElement::UpdateElement()
{
	Clear();

	if(IsSlotEmpty())
	{
		return;
	}

	const FItemSpec& ItemSpec = *GetItemSpec();;
	
	UpdateElement(ItemSpec);
}

void UWidgetItemElement::UpdateElement(const FItemSpec& spec)
{
	m_TextStackAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(spec);

	if(UMyLib::GetItemType(ItemData) == EItemType::Equip)
	{
		int Level = spec.m_nLevel;

		if(Level>0)
		{
			FString LevelStr = FString::Printf(TEXT("+ %d"),Level);

			m_TextStackAmount->SetText(FText::FromString(LevelStr));
		}
		else
		{
			m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);
		}

		if(UMyLib::GetEquip()->IsItemEquipped(spec))
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
		int StackCount = spec.m_nStack;

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
	return GetItemSpec()->IsEmpty();
}

int UWidgetItemElement::GetIndex()
{
	return m_nIndex;
}

void UWidgetItemElement::SetMyInteractable(bool isInteractAble)
{
	m_ElementBase->SetHoldable(isInteractAble);
}

void UWidgetItemElement::SetPanelType(EPanelType panel)
{
	m_PanelType = panel;
}