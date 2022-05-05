#include "WidgetEquipElement.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetEquipElement::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_ImgEquipIcon->SetBrushFromTexture(m_TextureEquipType);
}

void UWidgetEquipElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Slot = EEquipSlotType::None;

	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetEquipElement::OnHoldingComplete);

	m_ElementBase->m_OnClick.AddUObject(this,&UWidgetEquipElement::SetMyFocus);

	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);

	m_TextItemLevel->SetVisibility(ESlateVisibility::Collapsed);
}

//빈상태로 클릭했을때 필터링
FText UWidgetEquipElement::GetFocusText()
{
	if (!IsSlotEmpty())
	{
		return NSLOCTEXT("UWidgetEquipElement", "FocusUnEquip", "UnEquip?");
	}

	return FText();
}

void UWidgetEquipElement::OnHoldingComplete()
{
	FName ID = GetEquippedItem();

	UInventory* Inven = UMyLib::GetPlayerInven();
	
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Inven,ID,Inven);

	SetMyUnFocus();
}

void UWidgetEquipElement::SetSlot(EEquipSlotType slot)
{
	m_Slot = slot;
}

void UWidgetEquipElement::UpdateElement()
{
	const FName& Equipped = GetEquippedItem();
	
	if(!Equipped.IsNone()) 
	{
		m_ImgEquipIcon->SetVisibility(ESlateVisibility::Collapsed);
		
		UpdateElement(Equipped);//Update As Equipped
	}
	else//Update As UnEquipped
	{
		Clear();
	}
}

void UWidgetEquipElement::UpdateElement(const FName& spec)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(spec);

	m_OverlayEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	int Level = 0;//UMyLib::GetPlayerInven()->GetItemLevel(spec);;

	if(Level>0)
	{
		m_TextItemLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		FString LevelStr = FString::Printf(TEXT("+ %d"),Level);

		m_TextItemLevel->SetText(FText::FromString(LevelStr));
	}
	else
	{
		m_TextItemLevel->SetVisibility(ESlateVisibility::Collapsed);
	}

	m_ElementBase->SetIcon(ItemData.m_Icon);

	m_ElementBase->SetGlowColor(ItemData.m_ColorHandle);

	m_ElementBase->SetHoldable(true);
}

void UWidgetEquipElement::Clear()
{
	m_ElementBase->Clear();

	m_TextItemLevel->SetVisibility(ESlateVisibility::Collapsed);

	m_OverlayEquip->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgEquipIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ElementBase->SetHoldable(false);
}

void UWidgetEquipElement::UnEquipItem()
{
	UMyGameInstance::Get->m_ItemExeManager->EquipUnequipItem(GetEquippedItem());
}

void UWidgetEquipElement::SetMyFocus()
{
	if (IsSlotEmpty())
	{
		return; //필터링
	}

	if (m_TextFocus->IsVisible())
	{
		UnEquipItem(); //장착해제
		SetMyUnFocus();
		return;
	}

	m_TextFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextFocus->SetText(GetFocusText());

	m_ElementBase->SetMyFocus();
}

void UWidgetEquipElement::SetMyUnFocus()
{
	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ElementBase->SetMyUnFocus();
}

bool UWidgetEquipElement::IsSlotEmpty() const
{
	return !UMyLib::GetEquip()->IsItemEquipped(m_Slot);
}

FName UWidgetEquipElement::GetEquippedItem() const
{
	return UMyLib::GetEquip()->GetEquipItem(m_Slot);
}
