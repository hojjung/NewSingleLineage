#include "WidgetMapBtn.h"

#include "MyJrpg/DataTables/ZoneData.h"

void UWidgetMapBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnIcon->OnClicked.AddDynamic(this, &UWidgetMapBtn::OnClick);
	
	UpdateIcon();
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
}

void UWidgetMapBtn::OnClick()
{
	m_OnClick.ExecuteIfBound(m_ZoneID);
}

FName UWidgetMapBtn::GetZoneID()
{
	return  m_ZoneID;
}

void UWidgetMapBtn::SetZoneID(FName id, float dur)
{
	m_ZoneID = id;

	UpdateIcon();
}

FVector2D UWidgetMapBtn::GetPos()
{
	return Cast<UCanvasPanelSlot>( Slot)->GetPosition();
}
