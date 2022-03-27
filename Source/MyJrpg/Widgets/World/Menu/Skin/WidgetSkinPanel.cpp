// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSkinPanel.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetSkinPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetSkinPanel::OnClose);

	for(const FPlayerUnitEntityRow* PlSkin : UMyGameInstance::Get->m_AvatarManager->GetAllSkins())
	{
		UWidgetSkinElement* Ele = CreateWidget<UWidgetSkinElement>(this, m_ClassElement);

		Ele->Init(*PlSkin);

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
