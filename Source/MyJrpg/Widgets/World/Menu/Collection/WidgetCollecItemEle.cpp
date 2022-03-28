#include "WidgetCollecItemEle.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCollecItemEle::Init(const FName& collecID, int index, bool is_equip, const FItemDataHandle& item, int lv)
{
	m_Checkbox->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgLock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_bIsEquip = is_equip;

	m_bRegisterable = false;

	m_CollecID = collecID;

	m_ItemID = item.RowName;

	m_nIndex = index;

	m_nEnchantLv = lv;

	FString Str = FString::Printf(TEXT("+%d"),m_nEnchantLv);

	m_TextEnchantLevel->SetText(FText::FromString(Str));

	const FItemDataRow& ItemData = *item.GetRow<FItemDataRow>("");

	m_IconEle->SetIcon(ItemData.m_ItemIcon);

	m_IconEle->SetGlowColor(ItemData.m_ColorHandle);

	if(m_bIsEquip && lv > 0)
	{
		m_TextEnchantLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_TextEnchantLevel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidgetCollecItemEle::UpdateEquipItem()
{
	if(UMyLib::FindEquipItem(m_ItemID,0) != nullptr)
	{
		m_ImgLock->SetVisibility(ESlateVisibility::Collapsed);
		
		if(UMyLib::FindEquipItem(m_ItemID,m_nEnchantLv) != nullptr)
		{
			m_TextEnchantLevel->SetColorAndOpacity(FLinearColor::White);

			m_bRegisterable = true;
		}
		else
		{
			m_TextEnchantLevel->SetColorAndOpacity(FLinearColor::Red);
		}
	}
	else
	{
		m_TextEnchantLevel->SetColorAndOpacity(FLinearColor::White);

		m_ImgLock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetCollecItemEle::UpdateMiscItem()
{
	if(UMyLib::FindMiscItem(m_ItemID) != nullptr)
	{
		m_ImgLock->SetVisibility(ESlateVisibility::Collapsed);
		
		m_bRegisterable = true;
	}
	else
	{
		m_ImgLock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetCollecItemEle::Update()//포커싱이 되야지 등록을하잔아
{
	if(m_Checkbox->IsVisible())
	{
		return;//AlreadyDone
	}
	if (UMyGameInstance::Get->m_ItemCollecManager->IsItemRegistered(m_CollecID,m_nIndex))
	{
		m_IconEle->SetHoldable(false);

		m_IconEle->SetFocusable(false);

		m_Checkbox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		m_bIsRegistered = true;
		
		return;
	}
	m_bIsRegistered = false;

	m_IconEle->SetHoldable(true);

	m_IconEle->SetFocusable(true);

	m_bRegisterable = false;

	if (m_bIsEquip)
	{
		UpdateEquipItem();	
	}
	else
	{
		UpdateMiscItem();
	}

	
	
}

bool UWidgetCollecItemEle::GetIsRegistered()
{
	return m_bIsRegistered;
}
