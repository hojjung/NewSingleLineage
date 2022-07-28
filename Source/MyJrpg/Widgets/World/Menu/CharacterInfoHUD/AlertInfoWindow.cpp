#include "AlertInfoWindow.h"
#include "Components/VerticalBoxSlot.h"

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
		
		UVerticalBoxSlot* ChildSlot = Cast<UVerticalBoxSlot>(m_OverlayInfo->AddChild(ItemEle));
		
		ChildSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		
		ChildSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
		
		ChildSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		
		ItemEle->SetRenderScale(FVector2D(1.f,-1.f));
		
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
	m_OverlayInfo->ShiftChild(0, TextEle);
	return TextEle;
}

void UAlertInfoWindow::PrintInfoText(const FString& reason, FLinearColor color)
{
	GetTextElement()->Show(reason, color);
}
