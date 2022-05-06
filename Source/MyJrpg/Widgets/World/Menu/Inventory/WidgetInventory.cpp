#include "WidgetInventory.h"

#include "ItemDDO.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

FReply UWidgetInventory::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void UWidgetInventory::Init(UInventory* inven)
{
	check(m_ClassWidgetItemEle);

	m_InvenBox->ClearChildren();
	
	m_CurrentInven = inven;

	CreateGridElements();
}

void UWidgetInventory::OpenPanel()
{
	m_InvenDele = m_CurrentInven->m_OnInvenChanged.AddUObject(this, &UWidgetInventory::UpdateInventory);

	UpdateInventory();
}

void UWidgetInventory::ClosePanel()
{
	m_CurrentInven->m_OnInvenChanged.Remove(m_InvenDele);

	UnFocusCurrent();
}

void UWidgetInventory::CreateGridElements()
{
	check(m_ClassWidgetItemEle);

	m_AryItemEles.Reset();
	
	for (int i = 0; i < m_CurrentInven->GetInvenSize(); i++)
	{
		UWidgetBaseElement* ItemEle = CreateWidget<UWidgetBaseElement>(this, m_ClassWidgetItemEle);

		m_AryItemEles.Add(ItemEle);

		m_AryItemEles[i]->SetIndex(i);

		m_InvenBox->AddChildToWrapBox(m_AryItemEles[i])->SetPadding(FMargin(2));

		m_AryItemEles[i]->m_OnFocus.AddUObject(this,&UWidgetInventory::OnFocused);

		m_AryItemEles[i]->m_OnDrag.AddUObject(this,&UWidgetInventory::OnDrag);

		m_AryItemEles[i]->m_OnDrop.AddUObject(this,&UWidgetInventory::OnDrop);
	}
}

void UWidgetInventory::UpdateInventory()
{
	int Index = 0;

	for (const FItemSpec& Item : m_CurrentInven->GetAryItems())
	{
		if(Item.m_ID.IsNone())
		{
			m_AryItemEles[Index]->Clear();
			Index++;
			continue;
		}
		SetItem(m_AryItemEles[Index], Item);
		Index++;
	}
}

void UWidgetInventory::SetItem(UWidgetBaseElement* target, const FItemSpec& itemSpec)
{
	const FItemDataRow& Data = UMyLib::GetItemData(itemSpec.m_ID);

	target->SetIcon(Data.m_Icon);
	
	target->SetGlowColor(Data.m_ColorHandle);

	bool IsEquip = UMyLib::IsEquip(Data);

	if(IsEquip)
	{
		if(itemSpec.m_nLvStack > 0)
		{
			FString Str = FString::Printf(TEXT("+%d"), itemSpec.m_nLvStack);
			
			target->SetTextStackLv(Str);
		}
		else
		{
			target->HideTextStackLv();
		}

		target->SetEquip(itemSpec.m_bIsEquipped);
	}
	else
	{
		FString Str = FString::Printf(TEXT("%d"), itemSpec.m_nLvStack);
			
		target->SetTextStackLv(Str);
	}
}

void UWidgetInventory::UnFocusCurrent()
{
	if(m_CurrentFocused.Get())
	{
		m_CurrentFocused.Get()->SetMyUnFocus();

		m_CurrentFocused= nullptr;
	}
}

void UWidgetInventory::OnFocused(UWidgetBaseElement* ele)
{
	if(m_CurrentFocused.Get() && m_CurrentFocused.Get() == ele)
	{
		m_OnFocusConfirm.Broadcast(m_CurrentFocused.Get() ,m_CurrentInven.Get(), m_CurrentFocused->GetIndex());	
		UnFocusCurrent();
		return;
	}
	UnFocusCurrent();
	
	m_CurrentFocused = ele;

	m_OnFocus.Broadcast(m_CurrentFocused.Get() ,m_CurrentInven.Get(), m_CurrentFocused->GetIndex());
}

void UWidgetInventory::OnDrag(UWidgetBaseElement* ele)
{
	UnFocusCurrent();

	UItemDDO::GetDDOInst->m_FromInven = m_CurrentInven;

	UItemDDO::GetDDOInst->m_nIndex = ele->GetIndex();
}

void UWidgetInventory::OnDrop(UWidgetBaseElement* ele)
{
	UnFocusCurrent();
	
	if(UItemDDO::GetDDOInst->m_FromInven.Get())
	{
		m_CurrentInven->OnDropItem(ele->GetIndex(), UItemDDO::GetDDOInst->m_FromInven.Get(),UItemDDO::GetDDOInst->m_nIndex);
	}
}

UInventory* UWidgetInventory::GetInven() const
{
	return m_CurrentInven.Get();
}
