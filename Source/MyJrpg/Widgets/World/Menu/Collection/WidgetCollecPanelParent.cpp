// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetCollecPanelParent.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCollecPanelParent::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryUpdateElements.Init(FDelegateHandle(), FGlobalVariable::STORAGE_SIZE + 1);

	SetVisibility(ESlateVisibility::Collapsed);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetCollecPanelParent::OnClose);

	CreateElements();
	//
	
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

		for(UWidget* Ele : PanelChild->GetChildElements())
		{
			Cast<UWidgetCollecItemEle>(Ele)->m_OnFocus.AddUObject(this, &UWidgetCollecPanelParent::OnSelected);
		}

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

	m_TotalStat->UpdateTotalProgress();
	
	m_TotalStat->UpdateStats();
}

void UWidgetCollecPanelParent::ShowItemInfo(const FName& itemOid)
{
	
}

void UWidgetCollecPanelParent::OnSelected(UWidgetCollecItemEle* ele)
{
	if (m_CurrentFocused == ele)
	{
		return;
	}
	if (m_CurrentFocused)
		m_CurrentFocused->SetMyUnfocus();
	
	m_CurrentFocused = ele;

	m_CurrentFocused->SetMyFocus();
	
	ShowItemInfo(m_CurrentFocused->GetItemID());
}

void UWidgetCollecPanelParent::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_UpdateElements = UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements);

	m_AryUpdateElements[0] = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements);
	int Iter = 1;
	for(UInventory* Storage : UMyGameInstance::Get->m_AryStorage)
	{
		m_AryUpdateElements[Iter] = Storage->m_OnInvenChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements);

		Iter++;
	}

	UpdateElements();
}

void UWidgetCollecPanelParent::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.Remove(m_UpdateElements);

	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_AryUpdateElements[0]);
	int Iter = 1;
	for(UInventory* Storage : UMyGameInstance::Get->m_AryStorage)
	{
		Storage->m_OnInvenChanged.Remove(m_AryUpdateElements[Iter]);

		Iter++;
	}
}
