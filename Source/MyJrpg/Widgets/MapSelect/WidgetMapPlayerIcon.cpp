#include "WidgetMapPlayerIcon.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetMapPlayerIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_ZoneMove->m_OnMoveStart.AddUObject(this, &UWidgetMapPlayerIcon::ShowText);
	UMyGameInstance::Get->m_ZoneMove->m_OnMoveTick.AddUObject(this, &UWidgetMapPlayerIcon::UpdateDuration);
	UMyGameInstance::Get->m_ZoneMove->m_OnMoveEnd.AddUObject(this, &UWidgetMapPlayerIcon::HideText);

	m_TextDuration->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ImgBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ImgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	HideText();
}

void UWidgetMapPlayerIcon::HideText()
{
	m_ImgBG->SetVisibility(ESlateVisibility::Collapsed);
	
	m_TextDuration->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetMapPlayerIcon::ShowText(const FName& id, float dist)
{
	m_ImgBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextDuration->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetMapPlayerIcon::UpdateDuration()
{
	float Time = UMyGameInstance::Get->m_ZoneMove->GetRemainTime();

	const FString& CultName = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	
	FCulturePtr Culture = FInternationalization::Get().GetCulture(CultName);
	
	FTimespan RemainTimeSpan(0,0,Time);
	
	m_TextDuration->SetText(FText::AsTimespan(RemainTimeSpan, Culture));
}

