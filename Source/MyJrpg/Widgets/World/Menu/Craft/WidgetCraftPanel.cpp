#include "WidgetCraftPanel.h"
#include "WidgetCraftableElement.h"
#include "WidgetCraftCostElement.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"

void UWidgetCraftPanel::NativeOnInitialized()//Texture2D'/Game/Sprites/UI/HarmoBox/coin_01.coin_01'
{
	Super::NativeOnInitialized();
	
	CreateAllCraftWidget(UMyGameInstance::Get->m_CraftManager->GetAryCraftables());
}

void UWidgetCraftPanel::CreateAllCraftWidget(const TArray<FCraftable>& ary)
{
	int Index = 0;
	for(auto& CraftData : ary)
	{
		UWidgetCraftableElement* SelectButton = CreateWidget<UWidgetCraftableElement>(this,m_ClassCraftableElement);

		SelectButton->SetCraftable(CraftData,Index);

		SelectButton->m_OnClicked.AddUObject(this,&UWidgetCraftPanel::OnSelectCraftItem);

		m_GridEles->AddChild(SelectButton);

		Index++;
	}
}

void UWidgetCraftPanel::OnSelectCraftItem(const FCraftable& data, int index)
{
	UMyGameInstance::Get->m_CraftManager->SetCraftItem(index);

	m_Selected->SelectCraft(data);
}

void UWidgetCraftPanel::ClosePanel()
{
	Super::ClosePanel();
	
	m_Selected->Close();
	
	UMyGameInstance::Get->m_CraftManager->Clear();

	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_DeleUpdate);
}

void UWidgetCraftPanel::OpenPanel()
{
	Super::OpenPanel();

	//m_DeleUpdate = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this,&UWidgetCraftPanel::UpdateCraftCostPanel);
}