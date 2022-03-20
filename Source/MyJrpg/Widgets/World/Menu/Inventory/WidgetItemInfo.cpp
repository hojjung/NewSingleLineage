// All right Reserve 2021 HereticByte


#include "WidgetItemInfo.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"

void UWidgetItemInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nEraseAmount = 1;

	m_ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_ItemIcon->SetMyInteractable(false);
	
	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnClose);

	m_BtnClose->IsFocusable = false;

	m_BtnEraseItem->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnErase);

	m_BtnEraseItem->IsFocusable = false;

	

	m_BtnCalculator->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnOpenCalculator);
	
	m_BtnCalculator->IsFocusable = false;
}

void UWidgetItemInfo::SetItemInfo(const FItemSpec& itemSpecInfo)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_EquipItem = &itemSpecInfo;

	const FItemDataRow& ItemData = UMyLib::GetItemData(itemSpecInfo);

	EItemType Type = UMyLib::GetItemType(ItemData);

	switch (Type)
	{
	case EItemType::None:
		m_TextItemEffect->SetText(FText());
		break;
	case EItemType::Equip:
		m_BtnEraseItem->SetIsEnabled(!UMyLib::GetEquip()->IsItemEquipped(*m_EquipItem));
		m_TextItemEffect->SetText(FText());
		break;
	case EItemType::Consume:
		m_TextItemEffect->SetText(FText());
		break;
	}

	m_ItemIcon->UpdateElement(itemSpecInfo);
	
	m_TextItemName->SetText(ItemData.m_TextShowingName);

	m_TextItemDesc->SetText(ItemData.m_TextDesc);
	//
	
}

void UWidgetItemInfo::OnErase()
{
	//패널에서 버릴 숫자를 정할수 있어야한다.
	if(m_EquipItem)
	{
		UMyLib::GetPlayerInven()->RemoveItem(*m_EquipItem);
	}
	else if(m_ItemKey!=NAME_None)
	{
		UMyLib::GetPlayerInven()->RemoveItem(m_ItemKey,m_nEraseAmount);
	}

	OnClose();
}

void UWidgetItemInfo::OnOpenCalculator()
{
	UMyLib::GetCanvas()->OpenCalculator(0);
	
}

void UWidgetItemInfo::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
