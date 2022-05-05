#include "WidgetQuickSlot.h"

#include "WidgetQuickslotBar.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetCooldownProgress.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
//
void UWidgetQuickSlot::Init(int index, UWidgetQuickslotBar* slotParent)
{
	m_nIndex = index;
	
	m_ItemElement->SetFocusable(true);
	
	m_ItemElement->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_ParentBar = slotParent;

	UpdateQuickSlot();
}

void UWidgetQuickSlot::SetQuickSlotClear()
{
	m_ItemElement->Clear();
	m_TextItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	//UMyGameInstance::Get->m_SkillManager->Uneq(m_nIndex);
}

void UWidgetQuickSlot::UpdateQuickSlot()
{
	if (IsSlotEmpty())
	{
		m_ItemElement->Clear();
		m_TextItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UpdateSkillSlot();
	}
}

void UWidgetQuickSlot::UpdateSkillSlot()
{
	m_TextItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ItemElement->Clear();//데이터 테이블을 공유하지 않고 있는데 어떻게?
	
	m_ItemElement->SetIsEnabled(true);

	//m_ItemElement->SetIcon(GetSkillData()->m_SkillIcon);
}

const FSkillDataRow* UWidgetQuickSlot::GetSkillData()
{
	return UMyGameInstance::Get->m_SkillManager->GetSkillData(m_nIndex);
}

FReply UWidgetQuickSlot::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	UseQuick();

	return FReply::Handled();
}

FReply UWidgetQuickSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	UseQuick();

	return FReply::Handled();
}

bool UWidgetQuickSlot::IsSlotEmpty()
{
	return GetSkillData() == nullptr;
}

void UWidgetQuickSlot::OnSkillUseCountCD()
{
	float cd =  UMyGameInstance::Get->m_SkillManager->GetSkillCD(m_nIndex);
	
	m_CdProgress->StartCooldown(cd);
}

void UWidgetQuickSlot::UseQuick()
{
	const FName* CurrentSkill = m_ParentBar->GetSkill()->GetFocusedSkill();
	
	m_ParentBar->GetSkill()->UnFocusCurrent();
	
	if(CurrentSkill)
	{
		UMyGameInstance::Get->m_SkillManager->EquipSkill(m_nIndex, *CurrentSkill);
		
		return;
	}
	/////////////
	if(IsSlotEmpty())
	{
		return;
	}

	if (m_CdProgress->IsCooldown())
	{
		return;
	}

	if(!UMyGameInstance::Get->m_SkillManager->ExecuteSkill(m_nIndex))
	{
		return;
	}
}
