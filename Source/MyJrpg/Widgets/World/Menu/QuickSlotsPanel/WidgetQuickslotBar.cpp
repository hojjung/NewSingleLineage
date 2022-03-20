#include "WidgetQuickslotBar.h"

#include "WidgetQuickSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetQuickslotBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for(int i=0; i< FGlobalVariable::QUICKSLOT_MAX;i++)
	{
		UWidgetQuickSlot* ItemEle = CreateWidget<UWidgetQuickSlot>(this, m_ClassQuickSlot);

		ItemEle->Init(i,this);

		m_AryQuickSlot.Add(ItemEle);

		m_ScrollBarQuickSlot->AddChild(ItemEle);	
	}
	//QuickSlot Equip?
	UMyGameInstance::Get->m_SkillManager->m_OnSkillChanged.AddUObject(this, &UWidgetQuickslotBar::UpdateQuickSlots);

	UMyGameInstance::Get->m_SkillManager->m_OnSkillUse.AddUObject(this, &UWidgetQuickslotBar::OnStartCD);

	UpdateQuickSlots();
}

void UWidgetQuickslotBar::UpdateQuickSlots()
{
	for(auto QuickSlot : m_AryQuickSlot)
	{
		QuickSlot->UpdateQuickSlot();
	}
}

void UWidgetQuickslotBar::SetInvenSkill(UWidgetInventory* inven, UWidgetSkillPanel* skill)
{
	m_WidgetInven = inven;

	m_WidgetSkill = skill;
}

void UWidgetQuickslotBar::OnStartCD(int index)
{
	m_AryQuickSlot[index]->OnSkillUseCountCD();
}
