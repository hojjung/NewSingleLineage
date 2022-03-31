// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSkinPanel.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetSkinPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Preview->Init(UMyGameInstance::Get->m_AvatarManager,UMyGameInstance::Get->m_AvatarManager);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetSkinPanel::OnClose);

	m_BtnEquip->OnClicked.AddDynamic(this, &UWidgetSkinPanel::OnEquip);

	for(const FPlayerUnitEntityRow* PlSkin : UMyGameInstance::Get->m_AvatarManager->GetAllSkins())
	{
		UWidgetSkinElement* Ele = CreateWidget<UWidgetSkinElement>(this, m_ClassElement);

		Ele->Init(*PlSkin);

		Ele->m_OnFocus.AddUObject(this, &UWidgetSkinPanel::OnSelected);

		m_Wrap->AddChild(Ele);

		m_AryEles.Add(Ele);
	}
	
	UMyGameInstance::Get->m_AvatarManager->m_OnSkinChanged.AddUObject(this, &UWidgetSkinPanel::UpdateElements);
	
	UpdateElements();
}

void UWidgetSkinPanel::UpdateElements()
{
	for (UWidgetSkinElement* Ele : m_AryEles)
	{
		Ele->UpdateEle();
	}
}

void UWidgetSkinPanel::OnSelected(UWidgetSkinElement* ele)
{
	if (m_CurrentFocused == ele)
	{
		return;
	}
	if (m_CurrentFocused)
		m_CurrentFocused->SetMyUnfocus();
	
	m_CurrentFocused = ele;

	m_CurrentFocused->SetMyFocus();
	
	UMyGameInstance::Get->m_AvatarManager->ShowPreviewSkin(m_CurrentFocused->GetCrntSkin());
}

void UWidgetSkinPanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UMyGameInstance::Get->m_AvatarManager->ShowPawn();
}

void UWidgetSkinPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UMyGameInstance::Get->m_AvatarManager->HidePawn();
}

void UWidgetSkinPanel::OnEquip()
{
	if (!m_CurrentFocused)
	{
		return;
	}
	UMyGameInstance::Get->m_AvatarManager->EquipSkin(m_CurrentFocused->GetCrntSkin());
}
