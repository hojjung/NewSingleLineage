#include "WidgetMapBtn.h"

#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetMapBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnIcon->OnClicked.AddDynamic(this, &UWidgetMapBtn::OnClick);
	
	UpdateIcon();

	m_OverlayTime->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetMapBtn::UpdateIcon()
{
	const FZoneDataRow* ZoneData = UZoneData::GetZoneTable->FindRow<FZoneDataRow>(m_ZoneID, "");

	if(!ZoneData)
	{
		PRINTF("UWidgetMapBtn::UpdateIcon, No Data, ID Name is :%s", *m_ZoneID.ToString());
		return;	
	}
	FButtonStyle Style;
	UTexture2D* t = ZoneData->m_Icon.LoadSynchronous();
	Style.Normal.SetResourceObject(t);
	Style.Hovered.SetResourceObject(t);
	Style.Pressed.SetResourceObject(t);
	Style.Disabled.SetResourceObject(t);
	m_BtnIcon->SetStyle(Style);

	m_TextName->SetText(ZoneData->m_ShowingName);
}

void UWidgetMapBtn::OnClick()
{
	m_OnClick.ExecuteIfBound(m_ZoneID);
}

FName UWidgetMapBtn::GetZoneID()
{
	return  m_ZoneID;
}

void UWidgetMapBtn::UpdateDuration(float dur)
{
	if(dur <= 0.f)
	{
		m_OverlayTime->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	const FString& CultName = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	
	FCulturePtr Culture = FInternationalization::Get().GetCulture(CultName);
	
	FTimespan Run(0,0,dur);
	
	m_TextTimeRemain->SetText(FText::AsTimespan(Run, Culture));
}

void UWidgetMapBtn::SetZoneID(FName id, float dur)
{
	m_ZoneID = id;

	UpdateIcon();//시간 제한, 시간제한이 이동시간보다 크다면, 이동 불가능임

	m_OverlayTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UMyGameInstance::Get->m_EventStage->m_OnEventTick.AddUObject(this, &UWidgetMapBtn::UpdateDuration);

	UpdateDuration(dur);
}

FVector2D UWidgetMapBtn::GetPos()
{
	return Cast<UCanvasPanelSlot>( Slot)->GetPosition();
}
