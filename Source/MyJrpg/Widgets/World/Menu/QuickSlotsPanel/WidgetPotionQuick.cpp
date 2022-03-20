#include "WidgetPotionQuick.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPotionQuick::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_PotionManager->m_OnPotionChanged.AddUObject(this, &UWidgetPotionQuick::UpdatePotion);
	UMyGameInstance::Get->m_PotionManager->m_OnUsePotion.AddUObject(this, &UWidgetPotionQuick::OnUseCallback);
	UpdatePotion();
}

FReply UWidgetPotionQuick::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	UsePotion();

	return FReply::Handled();
}

void UWidgetPotionQuick::UsePotion()
{
	UMyGameInstance::Get->m_PotionManager->UsePotion();
}

void UWidgetPotionQuick::OnUseCallback()//for auto
{
	float Cd = UMyGameInstance::Get->m_PotionManager->GetMaxCD();

	m_CdProgress->StartCooldown(Cd);
}

void UWidgetPotionQuick::UpdatePotion()
{
	UPotionManager* Pm = UMyGameInstance::Get->m_PotionManager;
	
	FText Count = FText::AsNumber(Pm->GetCrntCnt());

	m_TextCount->SetText(Count);

	m_IconPotion->SetBrushFromTexture(Pm->GetPotionIcon());
}
