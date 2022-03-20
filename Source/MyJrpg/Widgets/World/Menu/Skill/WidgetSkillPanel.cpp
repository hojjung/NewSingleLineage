// All right Reserve 2021 HereticByte


#include "WidgetSkillPanel.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetSkillPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UpdateSkills();

	UMyGameInstance::Get->m_SkillManager->m_OnSkillChanged.AddUObject(this,&UWidgetSkillPanel::UpdateSkills);
}

void UWidgetSkillPanel::UpdateSkills()
{
	m_ScrollBox->ClearChildren();

	const TMap<FName, bool>& SkillLevels = UMyGameInstance::Get->m_SkillManager->GetSkillUnlocks();

	for(auto& SkillData : SkillLevels)
	{
		UWidgetSkillElement* ItemEle = CreateWidget<UWidgetSkillElement>(this, m_ClassSkillEle);

		ItemEle->SetSkill(SkillData.Key,SkillData.Value);

		ItemEle->m_OnFocus.AddUObject(this,&UWidgetSkillPanel::OnFocus);

		m_ScrollBox->AddChild(ItemEle);
		
		ItemEle->SetPadding(FMargin(0,30,0,0));
	}
}

void UWidgetSkillPanel::UnFocusCurrent()
{
	if(m_SelectedSkill.Get())
	{
		m_SelectedSkill.Get()->SetMyUnFocus();

		m_SelectedSkill= nullptr;
	}
}

void UWidgetSkillPanel::OnFocus(UWidgetSkillElement* ele)
{
	UnFocusCurrent();
	
	m_SelectedSkill = ele;
}

void UWidgetSkillPanel::ClosePanel()
{
	Super::ClosePanel();

	UnFocusCurrent();
}
