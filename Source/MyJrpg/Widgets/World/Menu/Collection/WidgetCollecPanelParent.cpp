#include "WidgetCollecPanelParent.h"

#include "MyJrpg/Managers/EquipManager.h"
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

	const TArray<FItemCollecRow*>& AryColleRows = UMyGameInstance::Get->m_ItemCollecManager->GetAryCollec();
	
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

	m_TotalStat->UpdateTotalProgress();
	
	m_TotalStat->UpdateStats();
}

void UWidgetCollecPanelParent::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_UpdateElements = UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements);

	m_AryUpdateElements.Reset(10);
	
	m_AryUpdateElements.Add(UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements));
	if(UMyGameInstance::Get->m_EquipManager->GetBag())
		m_AryUpdateElements.Add(UMyGameInstance::Get->m_EquipManager->GetBag()->m_OnInvenChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements));
	
	for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
		m_AryUpdateElements.Add(Storage->m_OnInvenChanged.AddUObject(this, &UWidgetCollecPanelParent::UpdateElements));

	UpdateElements();
}

void UWidgetCollecPanelParent::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_ItemCollecManager->m_OnCollecChanged.Remove(m_UpdateElements);

	int Iter = 0;
	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_AryUpdateElements[Iter++]);
	if(UMyGameInstance::Get->m_EquipManager->GetBag())
		UMyGameInstance::Get->m_EquipManager->GetBag()->m_OnInvenChanged.Remove(m_AryUpdateElements[Iter++]);
	
	for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
		Storage->m_OnInvenChanged.Remove(m_AryUpdateElements[Iter++]);
}