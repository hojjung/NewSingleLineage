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

	m_bNoFilter = true;

	m_AryItemEles.Reset();

	SetInventory(inven,panelType);
}

void UWidgetInventory::SetInventory(UInventory* inven,EPanelType panelType)
{
	m_InvenBox->ClearChildren();
	
	m_CurrentInven = inven;

	m_CurrentInven->m_OnInvenChanged.AddUObject(this, &UWidgetInventory::UpdateInventory);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetInventory::UpdateInventory);

	CreateGridElements(panelType);

	UpdateInventory();
}

void UWidgetInventory::CreateGridElements(EPanelType panelType)
{
	for (int i = 0; i < m_CurrentInven->m_nInvenMaxSize; i++)
	{
		UWidgetItemElement* ItemEle = CreateWidget<UWidgetItemElement>(this, m_ClassWidgetItemEle);

		ItemEle->SetPanelType(panelType);

		ItemEle->m_Inven = GetInven();

		m_AryItemEles.Add(ItemEle);
		
		m_AryItemEles[i]->Clear();

		m_InvenBox->AddChildToWrapBox(m_AryItemEles[i])->SetPadding(FMargin(2));

		m_AryItemEles[i]->m_OnFocus.AddUObject(this,&UWidgetInventory::OnFocused);
	}
}

void UWidgetInventory::UpdateInventory()
{
	int ItemIndex = 0;
	int Index = 0;
	
	for (const FItemSpec& Item : m_CurrentInven->GetAllItems())
	{
		if(!IsSameType(Item))
		{
			ItemIndex++;
			continue;
		}
		m_AryItemEles[Index]->SetIndex(ItemIndex);
		m_AryItemEles[Index]->UpdateElement();

		Index++;
		ItemIndex++;
	}

	for(; Index<m_AryItemEles.Num();Index++)
	{
		m_AryItemEles[Index]->Clear();
	}
	//남은 인벤들도 깨끗하게 해줘야한다
}

bool UWidgetInventory::IsSameType(const FItemSpec& item)
{
	if(m_bNoFilter)
	{
		return true;
	}
	
	EItemType Type = UMyLib::GetItemType(item.m_ItemID);
	
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

const FItemSpec* UWidgetInventory::GetFocusedItem() const
{
	return m_CurrentFocused.Get() ? m_CurrentFocused->GetItemSpec() : nullptr;
}

void UWidgetInventory::ClosePanel()
{
	UnFocusCurrent();
}

void UWidgetInventory::SetItemFilter(EItemType typeWant)
{
	UnFocusCurrent();
	
	m_bNoFilter = false;

	m_FilterCategoryItem = typeWant;

	UpdateInventory();
}

void UWidgetInventory::ClearFilter()
{
	UnFocusCurrent();
	
	m_bNoFilter = true;

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