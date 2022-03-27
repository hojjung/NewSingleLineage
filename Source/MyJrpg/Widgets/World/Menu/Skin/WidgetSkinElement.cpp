#include "WidgetSkinElement.h"

#include "MyJrpg/Managers/MyGameInstance.h"


void UWidgetSkinElement::Init(const FPlayerUnitEntityRow& pl)
{
	m_Skin = &pl;
	
	m_AvatarManager = UMyGameInstance::Get->m_AvatarManager;

	m_Ele->SetFocusable(true);
	m_Ele->SetHoldable(false);
	m_Ele->SetGlowColor(m_Skin->m_Color);
	m_Ele->SetIcon(m_Skin->m_Icon);
	//m_ImgWeapon
	UpdateEle();
}

void UWidgetSkinElement::UpdateEle()
{
	if (m_AvatarManager->GetCrntSkin() == m_Skin)
	{
		m_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_Selected->SetVisibility(ESlateVisibility::Collapsed);
	}
}
