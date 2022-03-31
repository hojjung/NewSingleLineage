#include "WidgetPetElement.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPetElement::Init(const FPetRow& pl)
{
	m_Pet = &pl;
	m_PetManager = UMyGameInstance::Get->m_PetManager;
	m_Ele->SetFocusable(true);
	m_Ele->SetHoldable(false);
	m_Ele->SetGlowColor(m_Pet->m_Color);
	m_Ele->SetIcon(m_Pet->m_Icon);
	m_Ele->m_OnClick.AddUObject(this,&UWidgetPetElement::OnClicked);

	UpdateEle();
}

void UWidgetPetElement::UpdateEle()
{
	if (m_PetManager->GetCrntPet() == m_Pet)
	{
		m_FocusParent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		m_TextFocus->SetText(NSLOCTEXT("UWidgetPetElement","Equipped","장착됨"));
		return;
	}
	m_FocusParent->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetPetElement::SetMyUnfocus()
{
	m_Ele->SetMyUnFocus();
}

void UWidgetPetElement::SetMyFocus()
{
	m_Ele->SetMyFocus();

	UpdateEle();
}

void UWidgetPetElement::SetConfirm()
{
	bool IsEquipped = m_PetManager->GetCrntPet() == m_Pet;

	if (IsEquipped)
	{
		//장착해제
		//기본 변신으로 돌아감
		return;
	}
	//언락이 된 변신이라면 변신,아니라면 애초에 장착을 묻지도 말아야함
}

const FPetRow& UWidgetPetElement::GetCrntPet()
{
	return *m_Pet;
}

void UWidgetPetElement::OnClicked()
{
	m_OnFocus.Broadcast(this);
}
