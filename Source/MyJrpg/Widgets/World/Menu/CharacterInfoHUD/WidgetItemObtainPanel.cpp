#include "WidgetItemObtainPanel.h"
#include "WidgetItemObtainElement.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"
#include "Components/OverlaySlot.h"

void UWidgetItemObtainPanel::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();

	check(m_ClassInfo);
	
	m_AryInfo.Reset();

	int	iter = 0;

	while (iter < 10)
	{
		UAlertInfoElement* ItemEle = CreateWidget<UAlertInfoElement>(this, m_ClassInfo);

		ItemEle->Hide();
		
		m_AryInfo.Add(ItemEle);
		
		UVerticalBoxSlot* ChildSlot = Cast<UVerticalBoxSlot>(m_OverlayInfo->AddChild(ItemEle));
		
		ChildSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		
		ChildSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);

		ItemEle->SetRenderScale(FVector2D(1.f,-1.f));
		
		iter++;
	}

	UMyGameInstance::Get->m_EquipManager->m_OnItemEarn.AddUObject(this, &UWidgetItemObtainPanel::OnItemObtain);
}

void UWidgetItemObtainPanel::OnItemObtain(const FItemDataRow& item, int amount)
{	//겹칠때 어떻게 딜레이 시켜줌?
	Cast<UWidgetItemObtainElement>(GetTextElement())->ShowImageText(item,amount);
}