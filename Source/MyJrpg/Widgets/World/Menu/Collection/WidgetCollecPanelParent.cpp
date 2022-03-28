// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetCollecPanelParent.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCollecPanelParent::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetCollecPanelParent::OnClose);

	CreateElements();
}

void UWidgetCollecPanelParent::CreateElements()
{
	int Iter = 0;
	
	auto AryColleRows = UMyGameInstance::Get->m_ItemCollecManager->GetAryCollec();
	
	for(const FName& ItemCollec : UMyGameInstance::Get->m_ItemCollecManager->GetAryCollecKeys())
	{
		UWidgetCollecPanelChild* PanelChild = CreateWidget<UWidgetCollecPanelChild>(this, m_ClassChild);
		
		PanelChild->Init(ItemCollec, *AryColleRows[Iter]);

		PanelChild->Update();

		m_Scroll->AddChild(PanelChild);

		Iter++;
	}
}

void UWidgetCollecPanelParent::UpdateElements()
{
	for(UWidget* ChildWidget : m_Scroll->GetAllChildren())
	{
		UWidgetCollecPanelChild* CollecChild = Cast<UWidgetCollecPanelChild>(ChildWidget);

		CollecChild->Update();
	}
}

void UWidgetCollecPanelParent::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_UpdateElements = UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements);

	m_UpdateStats = UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.AddUObject(m_TotalStat, &UWidgetCollecStatParent::UpdateStats);

	m_UpdateTotalProgress = UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.AddUObject(m_TotalStat, &UWidgetCollecStatParent::UpdateTotalProgress);

	m_TotalStat->UpdateTotalProgress();
	
	m_TotalStat->UpdateStats();

	UpdateElements();
}

void UWidgetCollecPanelParent::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.Remove(m_UpdateStats);

	UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.Remove(m_UpdateTotalProgress);

	UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.Remove(m_UpdateElements);
}
