// All right Reserve 2021 HereticByte


#include "WidgetEquipInvenPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetEquipInvenPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyLib::GetPlayerInven()->m_OnInvenChanged.AddUObject(this,&UWidgetEquipInvenPanel::UpdateText);

	m_InvenPanel->Init(UMyLib::GetPlayerInven());
	
	m_FilterBtns->RegisterFilter(m_InvenPanel);

	UpdateText();
}

void UWidgetEquipInvenPanel::UpdateText()
{
	int CurrentCount = 0;//UMyLib::GetPlayerInven()->GetEmptyIndex();

	int MaxCount = UMyLib::GetPlayerInven()->GetInvenSize();

	FString StrP = FString::Printf(TEXT("%d/%d"),CurrentCount,MaxCount);

	m_TxtInvenCount->SetText(FText::FromString(StrP));
}

void UWidgetEquipInvenPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	m_InvenPanel->ClosePanel();
}

UWidgetInventory* UWidgetEquipInvenPanel::GetInvenPanel()
{
	return m_InvenPanel;	
}

UWidgetEquipPanel* UWidgetEquipInvenPanel::GetEquipPanel()
{
	return m_EquipPanel;
}

UWidgetStatPanel* UWidgetEquipInvenPanel::GetStatPanel()
{
	return m_StatPanel;
}
