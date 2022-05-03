#include "WidgetMapBtn.h"

#include "MyJrpg/DataTables/ZoneData.h"

void UWidgetMapBtn::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(m_TextureIcon)
	{
		FButtonStyle Style;

		Style.Normal.SetResourceObject(m_TextureIcon);
		Style.Hovered.SetResourceObject(m_TextureIcon);
		Style.Pressed.SetResourceObject(m_TextureIcon);
		Style.Disabled.SetResourceObject(m_TextureIcon);
		
		m_BtnIcon->SetStyle(Style);
	}
}

void UWidgetMapBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnIcon->OnClicked.AddDynamic(this, &UWidgetMapBtn::OnClick);
}

void UWidgetMapBtn::OnClick()
{
	m_OnClick.ExecuteIfBound(m_ZoneID);
}
