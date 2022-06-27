#include "AlertInfoWindow.h"

#include "Components/OverlaySlot.h"

void UAlertInfoWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(m_ClassInfo);
	
	m_AryInfo.Reset();

	int	iter = 0;

	while (iter < 10)
	{
		UAlertInfoElement* ItemEle = CreateWidget<UAlertInfoElement>(this, m_ClassInfo);

		ItemEle->Hide();
		
		m_AryInfo.Add(ItemEle);
		
		UOverlaySlot* ChildSlot = Cast<UOverlaySlot>(m_OverlayInfo->AddChild(ItemEle));
		
		ChildSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		
		ChildSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
		
		iter++;
	}
}

UAlertInfoElement* UAlertInfoWindow::GetTextElement()
{
	UAlertInfoElement* TextEle = m_AryInfo[m_nIndex];

	m_nIndex++;

	if (m_nIndex >= m_AryInfo.Num())
	{
		m_nIndex = 0;
	}

	return TextEle;
}

void UAlertInfoWindow::PrintErrorText(const FString& reason)
{
	GetTextElement()->Show(reason);
}
