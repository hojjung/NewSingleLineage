#include "WidgetSkinElement.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetSkinElement::Init(const FPlayerUnitEntityRow& pl)
{
	m_Skin = &pl;
	m_AvatarManager = UMyGameInstance::Get->m_AvatarManager;
	m_Ele->SetFocusable(true);
	m_Ele->SetHoldable(false);
	//m_Ele->SetGlowColor(m_Skin->m_Color);
	//m_Ele->SetIcon(m_Skin->m_Icon);

	UpdateEle();
}

void UWidgetSkinElement::UpdateEle()
{
	if (m_AvatarManager->GetCrntSkin() == m_Skin)
	{
		m_FocusParent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		m_TextFocus->SetText(NSLOCTEXT("UWidgetSkinElement","Equipped","장착됨"));
		return;
	}
	m_FocusParent->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkinElement::SetMyUnfocus()
{
	m_Ele->SetMyUnFocus();
}

void UWidgetSkinElement::SetMyFocus()
{
	m_Ele->SetMyFocus();

	UpdateEle();
}

const FPlayerUnitEntityRow& UWidgetSkinElement::GetCrntSkin()
{
	return *m_Skin;
}

void UWidgetSkinElement::OnClicked()
{
	m_OnFocus.Broadcast(this);
}
