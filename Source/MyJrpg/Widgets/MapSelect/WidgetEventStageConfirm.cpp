#include "WidgetEventStageConfirm.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetEventStageConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnLook->OnClicked.AddDynamic(this, &UWidgetEventStageConfirm::OnLook);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetEventStageConfirm::OnClose);
}

void UWidgetEventStageConfirm::SetStageConfirm(const FEventStageSpec& data)
{
	m_DataRow = &data;
	
	m_ImgEvent->SetBrushFromSoftTexture(m_DataRow->m_EventDataRow->m_EventImage);

	m_TextDesc->SetText(m_DataRow->m_EventDataRow->m_TextEventDesc);

	UWidgetMapBtn* MapBtn = UMyGameInstance::Get->m_ZoneMove->GetMapBtn(m_DataRow->m_EventDataRow->m_ZoneID);

	m_BtnPosCached = MapBtn->GetPos();
}

void UWidgetEventStageConfirm::OnLook()
{
	UMyLib::GetMapCanvas()->SetMapCanvasPos(m_BtnPosCached, true);

	OnClose();
}

void UWidgetEventStageConfirm::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	RemoveFromParent();
}
