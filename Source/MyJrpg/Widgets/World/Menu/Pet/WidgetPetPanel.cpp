#include "WidgetPetPanel.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPetPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	m_Preview->Init(UMyGameInstance::Get->m_PetManager,UMyGameInstance::Get->m_PetManager);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClose);

	m_BtnEquip->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnEquip);

	for(const FPetRow* Pet : UMyGameInstance::Get->m_PetManager->GetPetDatas())
	{
		UWidgetPetElement* Ele = CreateWidget<UWidgetPetElement>(this, m_ClassElement);

		Ele->Init(*Pet);

		Ele->m_OnFocus.AddUObject(this, &UWidgetPetPanel::OnSelected);

		m_Wrap->AddChild(Ele);

		m_AryEles.Add(Ele);
	}
	
	UMyGameInstance::Get->m_PetManager->m_OnPetChanged.AddUObject(this, &UWidgetPetPanel::UpdateElements);
	
	UpdateElements();
}

void UWidgetPetPanel::UpdateElements()
{
	for (UWidgetPetElement* Ele : m_AryEles)
	{
		Ele->UpdateEle();
	}
}

void UWidgetPetPanel::OnSelected(UWidgetPetElement* ele)
{
	if (m_CurrentFocused == ele)
	{
		return;
	}
	if (m_CurrentFocused)
		m_CurrentFocused->SetMyUnfocus();
	
	m_CurrentFocused = ele;

	m_CurrentFocused->SetMyFocus();
	
	UMyGameInstance::Get->m_PetManager->ShowPreviewPet(m_CurrentFocused->GetCrntPet());
}

void UWidgetPetPanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UMyGameInstance::Get->m_PetManager->ShowPawn();
}

void UWidgetPetPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UMyGameInstance::Get->m_PetManager->HidePawn();
}

void UWidgetPetPanel::OnEquip()
{
	if (!m_CurrentFocused)
	{
		return;
	}
	if(&m_CurrentFocused->GetCrntPet() && UMyGameInstance::Get->m_PetManager->GetCrntPet())
	{
		UMyGameInstance::Get->m_PetManager->UnEquip();
		return;
	}
	UMyGameInstance::Get->m_PetManager->EquipPet(m_CurrentFocused->GetCrntPet());
}
