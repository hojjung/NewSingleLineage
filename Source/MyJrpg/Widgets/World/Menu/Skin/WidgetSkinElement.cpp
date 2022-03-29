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
	m_Ele->m_OnClick.AddUObject(this,&UWidgetSkinElement::OnClicked);

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

void UWidgetSkinElement::SetConfirm()
{
	bool IsEquipped = m_AvatarManager->GetCrntSkin() == m_Skin;

	if (IsEquipped)
	{
		//장착해제
		//기본 변신으로 돌아감
		return;
	}
	//언락이 된 변신이라면 변신,아니라면 애초에 장착을 묻지도 말아야함
}

const FPlayerUnitEntityRow& UWidgetSkinElement::GetCrntSkin()
{
	return *m_Skin;
}

void UWidgetSkinElement::OnClicked()
{
	m_OnFocus.Broadcast(this);
}
