#include "WidgetInventory.h"

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

void UWidgetInventory::Init(UInventory* inven,EPanelType panelType)
{
	check(m_ClassWidgetItemEle);

	m_AryItemEles.Reset();

	m_InvenBox->ClearChildren();
	
	m_CurrentInven = inven;

	CreateGridElements(panelType);
}

void UWidgetInventory::OpenPanel()
{
	m_InvenDele = m_CurrentInven->m_OnInvenChanged.AddUObject(this, &UWidgetInventory::UpdateInventory);

	m_EquipDele = UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetInventory::UpdateInventory);

	UpdateInventory();
}

void UWidgetInventory::ClosePanel()
{
	m_CurrentInven->m_OnInvenChanged.Remove(m_InvenDele);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.Remove(m_EquipDele);
	
	UnFocusCurrent();
}

void UWidgetInventory::CreateGridElements(EPanelType panelType)
{
	check(m_ClassWidgetItemEle);
	
	for (int i = 0; i < m_CurrentInven->GetInvenSize(); i++)
	{
		UWidgetItemElement* ItemEle = CreateWidget<UWidgetItemElement>(this, m_ClassWidgetItemEle);

		ItemEle->Init(panelType, GetInven());

		m_AryItemEles.Add(ItemEle);
		
		m_InvenBox->AddChildToWrapBox(m_AryItemEles[i])->SetPadding(FMargin(2));

		m_AryItemEles[i]->m_OnFocus.AddUObject(this,&UWidgetInventory::OnFocused);
	}
}

void UWidgetInventory::UpdateInventory()
{
	int ItemIndex = 0;
	int Index = 0;
	
	for (const FName& Item : m_CurrentInven->GetAryTotalItemIDs())
	{
		if(!IsSameType(Item))
		{
			ItemIndex++;
			continue;
		}
		m_AryItemEles[Index]->SetIndex(ItemIndex);
		Index++;
		ItemIndex++;
	}

	for(; Index<m_AryItemEles.Num();Index++)
	{
		m_AryItemEles[Index]->Clear();
	}
}

bool UWidgetInventory::IsSameType(const FName& item)
{
	if(m_FilterCategoryItem == EItemType::None)
	{
		return true;
	}
	
	EItemType Type = UMyLib::GetItemType(item);
	
	return Type == m_FilterCategoryItem;
}

void UWidgetInventory::UnFocusCurrent()
{
	if(m_CurrentFocused.Get())
	{
		m_CurrentFocused.Get()->SetMyUnFocus();

		m_CurrentFocused= nullptr;
	}
}

void UWidgetInventory::OnFocused(UWidgetItemElement* ele)
{
	UnFocusCurrent();
	
	m_CurrentFocused = ele;
}

UInventory* UWidgetInventory::GetInven() const
{
	return m_CurrentInven.Get();
}

void UWidgetInventory::SetItemFilter(EItemType typeWant)
{
	UnFocusCurrent();
	
	m_FilterCategoryItem = typeWant;

	UpdateInventory();
}

void UWidgetInventory::ClearFilter()
{
	UnFocusCurrent();
	
	m_FilterCategoryItem = EItemType::None;

	UpdateInventory();
}

void UWidgetInventory::OnFilterMisc()
{
	SetItemFilter(EItemType::None);
}

void UWidgetInventory::OnFilterConsumable()
{
	SetItemFilter(EItemType::Consume);
}

void UWidgetInventory::OnFilterEquips()
{
	SetItemFilter(EItemType::Equip);
}