#include "WidgetLogReply.h"
#include "WidgetLogReply.h"
#include "WidgetLogWindow.h"
#include "MyJrpg/MyLib.h"

void UWidgetLogReply::Init(const FDialogueNode& node,UWidgetLogWindow* log)
{
	m_ReplyNode = &node;

	FString StringMade =  log->ReplaceVarStrings(m_ReplyNode->m_Text);
	
	m_TextReply->SetText(FText::FromString(StringMade));

	if(node.m_bIsPlayer)
	{
		m_SpeakerPortrait->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(!node.m_SpeakerIDNode.IsNone())
	{
		FEntityRow* EntityRow = UUnitEntityData::GetNpcUnitTable->FindRow<FEntityRow>(node.m_SpeakerIDNode, "");
		
		m_SpeakerIcon->SetBrushFromSoftTexture(EntityRow->m_Icon);

		m_SpeakerName->SetText(EntityRow->m_ShowingName);
	}
}

FReply UWidgetLogReply::OnClick(const FPointerEvent& inputEvent)
{
	FVector2D MousePos = inputEvent.GetScreenSpacePosition();

	if(m_TextReply->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		m_OnClick.Broadcast(*m_ReplyNode);
	
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UWidgetLogReply::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return OnClick(InGestureEvent);
}

FReply UWidgetLogReply::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	return OnClick(InMouseEvent);
}
