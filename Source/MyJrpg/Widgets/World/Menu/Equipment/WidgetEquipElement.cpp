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

	int Level = 0;//UMyLib::GetPlayerInven()->GetItemLevel(spec);;

	if(Level>0)
	{
		FString LevelStr = FString::Printf(TEXT("+ %d"),Level);
	}
	else
	{
		
	}

	//m_ElementBase->SetItem(ItemData);

	m_ElementBase->SetHoldable(true);
}

void UWidgetEquipElement::Clear()
{
	m_ElementBase->Clear();

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

	m_ElementBase->SetMyFocus();
}

void UWidgetEquipElement::SetMyUnFocus()
{
	
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
