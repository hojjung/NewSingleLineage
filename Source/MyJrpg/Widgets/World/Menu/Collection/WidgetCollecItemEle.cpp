#include "WidgetCollecItemEle.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCollecItemEle::Init(const FName& collecID, int index, bool is_equip, const FItemDataHandle& item, int lv)
{
	m_Checkbox->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgLock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_IconEle->SetHoldable(true);

	m_IconEle->m_OnHold.AddUObject(this,&UWidgetCollecItemEle::OnHoldComplete);

	m_IconEle->m_OnClick.AddUObject(this,&UWidgetCollecItemEle::OnClicked);

	m_bIsEquip = is_equip;

	m_bRegisterable = false;

	m_CollecID = collecID;

	m_ItemID = item.RowName;

	m_nIndex = index;

	m_nEnchantLv = lv;

	FString Str = FString::Printf(TEXT("+%d"),m_nEnchantLv);

	m_TextEnchantLevel->SetText(FText::FromString(Str));

	const FItemDataRow& ItemData = *item.GetRow<FItemDataRow>("");

	m_IconEle->SetIcon(ItemData.m_Icon);

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

void UWidgetCollecItemEle::OnHoldComplete()
{
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Inven,m_ItemID,nullptr);
}

void UWidgetCollecItemEle::UpdateEquipItem()
{
	if(UMyLib::FindEquipItem(m_ItemID) != nullptr)
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
		m_IconEle->SetFocusable(false);

		m_IconEle->SetHoldable(false);

		m_Checkbox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		m_bIsRegistered = true;

		SetMyUnfocus();

		m_ImgLock->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		return;
	}
	m_bIsRegistered = false;

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

const FName& UWidgetCollecItemEle::GetItemID() const
{
	return m_ItemID;
}

const FName& UWidgetCollecItemEle::GetCollecID() const
{
	return m_CollecID;
}

int UWidgetCollecItemEle::GetItemIndex() const
{
	return m_nIndex;
}

void UWidgetCollecItemEle::SetMyUnfocus()
{
	m_IconEle->SetMyUnFocus();
}

void UWidgetCollecItemEle::SetMyFocus()
{
	m_IconEle->SetMyFocus();
}

void UWidgetCollecItemEle::OnClicked()
{
	m_OnFocus.Broadcast(this);
}
