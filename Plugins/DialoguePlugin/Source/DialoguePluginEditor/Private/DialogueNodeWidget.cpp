#include "DialogueNodeWidget.h"
#include "DialoguePluginEditorPrivatePCH.h"
#include "DialogueEditor.h"
#include "DialogueViewportWidget.h"
#include "DialogueEditorStyle.h"
#include "Dialogue.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Layout/WidgetPath.h"
#include "Framework/Application/MenuStack.h"
#include "Editor/UnrealEd/Public/ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "SDialogueViewportWidget"
 
void SDialogueNodeWidget::Construct(const FArguments& InArgs)
{	
	Id = InArgs._Id;
	NodeIndex = InArgs._NodeIndex;
	Dialogue = InArgs._Dialogue; // for styles, TD: move somewhere?
	Owner = InArgs._Owner;
	Visibility = TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &SDialogueNodeWidget::GetNodeVisibility));
	
	ChildSlot
	[
		SNew(SOverlay)
			/*
			* NodeImage
			*/
			+ SOverlay::Slot() // the node itself
			[
				SNew(SImage)
				.Image(this, &SDialogueNodeWidget::GetNodeStyle)
				.Visibility(EVisibility::Visible)
			]

			/*
			* a Spacer that gives a minimum size to the node
			*/
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(SSpacer)
				.Size(TAttribute<FVector2D>::Create(TAttribute<FVector2D>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetNodeMinSize)))
			]

			/*
			* horizontal box containing top left icons
			*/
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetLeftCornerPadding))) //.Padding(5, 0, 0, 0) at Zoom 1:1
			[
				SNew(SHorizontalBox)
				.RenderTransform(TAttribute<TOptional<FSlateRenderTransform>>::Create(TAttribute<TOptional<FSlateRenderTransform>>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetIconScale)))
				.RenderTransformPivot(FVector2D(0, 0))
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5, 3, 0, 0)
				[
					SNew(SImage)
					.Image(FDialogueEditorStyle::Get()->GetBrush("EventIcon"))
					.ColorAndOpacity(FLinearColor(246.0f / 255.0f, 207.0f / 255.0f, 6.0f / 255.0f, 1.0f))
					.Visibility(this, &SDialogueNodeWidget::GetEventIconVisibility)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5, 3, 0, 0)
				[
					SNew(SImage)
					.Image(FDialogueEditorStyle::Get()->GetBrush("ConditionIcon"))
					.ColorAndOpacity(FLinearColor(106.0f / 255.0f, 221.0f / 255.0f, 214.0f / 255.0f, 1.0f)) // FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
					.Visibility(this, &SDialogueNodeWidget::GetConditionIconVisibility)
				]
			]

			/*
			* horizontal box containing top right icons
			*/
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Right)
			.Padding(0, 0, 0, 0) // left top right bottom
			[
				SNew(SHorizontalBox)
				.RenderTransform(TAttribute<TOptional<FSlateRenderTransform>>::Create(TAttribute<TOptional<FSlateRenderTransform>>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetIconScale)))
				.RenderTransformPivot(FVector2D(1, 0))
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.FillWidth(1)
				.Padding(0, 3, 10, 0)
				[
					SNew(SImage)
					.Image(FDialogueEditorStyle::Get()->GetBrush("VoiceIcon"))
					.ColorAndOpacity(FLinearColor(106.0f / 255.0f, 174.0f / 255.0f, 101.0f / 255.0f, 1.0f))
					.Visibility(this, &SDialogueNodeWidget::GetSoundIconVisibility)
				]
			]

			/*
			* text block - displayed only when out of Editing mode
			*/
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(TAttribute<FMargin>(this, &SDialogueNodeWidget::GetTextMargin)) // left top right bottom
			[
				SAssignNew(NodeTextBlock, STextBlock)
				.Justification(ETextJustify::Left)
				.Text(this, &SDialogueNodeWidget::GetNodeText)
				.Visibility(this, &SDialogueNodeWidget::GetTextBlockVisibility) // @TODO: make a getter?
				.WrapTextAt(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetNodeTextWrapLength))) // 250 by default at 1:1 zoom level
				.Font(TAttribute<FSlateFontInfo>::Create(TAttribute<FSlateFontInfo>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetNodeFont)))
			]

			/*
			* editable text - displayed only when in Editing mode
			*/
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(TAttribute<FMargin>(this, &SDialogueNodeWidget::GetTextMargin))
			[
				SAssignNew(NodeField, SMultiLineEditableText)
				.Text(this, &SDialogueNodeWidget::GetFieldText)
				.Visibility(this, &SDialogueNodeWidget::GetTextFieldVisibility)
				.WrapTextAt(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetNodeTextWrapLength))) // 250 by default at 1:1 zoom level
				.ModiferKeyForNewLine(EModifierKey::Shift)
				.OnTextCommitted(this, &SDialogueNodeWidget::TextCommited, Id)
				.SelectAllTextWhenFocused(true)
				.Font(TAttribute<FSlateFontInfo>::Create(TAttribute<FSlateFontInfo>::FGetter::CreateSP(Owner, &SDialogueViewportWidget::GetNodeFont)))
			]
	];
}

/*
* Performs culling. If the node isn't on screen, then it's collapsed. Boosts performance drammatically for dialogues with a large number of nodes.
*/
EVisibility SDialogueNodeWidget::GetNodeVisibility() const
{
	return isVisible ? EVisibility::Visible : EVisibility::Collapsed;
}

FMargin SDialogueNodeWidget::GetTextMargin() const
{
	if (!isVisible)
	{
		return FMargin(0, 0, 0, 0);
	}

	float marg10 = 10 * Owner->GetZoomAmount();
	float marg17 = 16 * Owner->GetZoomAmount();

	if (Owner->GetZoomLevel() <= 4)
	{
		return FMargin(0, 0, 0, 0);
	}	

	if (Owner->GetZoomLevel() > 6 && (Dialogue->m_AryData[NodeIndex].m_AryEvents.Num() > 0 || Dialogue->m_AryData[NodeIndex].m_AryConditions.Num() > 0 || Dialogue->m_AryData[NodeIndex].m_Sound || Dialogue->m_AryData[NodeIndex].m_DialogueWave))
	{
		return FMargin(marg10, marg17, marg10, marg10);
	}
	else
	{
		return FMargin(marg10, marg10, marg10, marg10);
	}
}

EVisibility SDialogueNodeWidget::GetTextFieldVisibility() const
{
	if (!isVisible)
	{
		return EVisibility::Collapsed;
	}

	if (Owner->GetZoomLevel() <= 6)
	{
		return EVisibility::Collapsed;
	}

	return (Owner->EditTextNode == Id) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SDialogueNodeWidget::GetEventIconVisibility() const
{
	if (!isVisible)
	{
		return EVisibility::Collapsed;
	}

	if (Owner->GetZoomLevel() <= 6)
	{
		return EVisibility::Collapsed;
	}

	return Dialogue->m_AryData[NodeIndex].m_AryEvents.Num() > 0 ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}

EVisibility SDialogueNodeWidget::GetConditionIconVisibility() const
{
	if (!isVisible)
	{
		return EVisibility::Collapsed;
	}

	if (Owner->GetZoomLevel() <= 6)
	{
		return EVisibility::Collapsed;
	}

	return Dialogue->m_AryData[NodeIndex].m_AryConditions.Num() > 0 ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}

EVisibility SDialogueNodeWidget::GetSoundIconVisibility() const
{
	if (!isVisible)
	{
		return EVisibility::Collapsed;
	}

	if (Owner->GetZoomLevel() <= 6)
	{
		return EVisibility::Collapsed;
	}

	return (Dialogue->m_AryData[NodeIndex].m_Sound || Dialogue->m_AryData[NodeIndex].m_DialogueWave) ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}

EVisibility SDialogueNodeWidget::GetTextBlockVisibility() const
{
	if (!isVisible)
	{
		return EVisibility::Collapsed;
	}

	if (Owner->GetZoomLevel() <= 4)
	{
		return EVisibility::Collapsed;
	}

	if (Owner->GetZoomLevel() <= 6)
	{
		return EVisibility::Hidden;
	}

	return (Owner->EditTextNode == Id) ? EVisibility::Collapsed : EVisibility::Visible;
}

void SDialogueNodeWidget::TextCommited(const FText& NewText, ETextCommit::Type CommitInfo, int32 id)
{
	if (NewText.EqualTo(Dialogue->m_AryData[NodeIndex].m_Text)) return;
	const FScopedTransaction Transaction(LOCTEXT("TextCommited", "Edited Node Text"));
	Dialogue->Modify();
	Dialogue->m_AryData[NodeIndex].m_Text = NewText;
	Owner->EditTextNode = -1;
	Owner->DialogueEditorPtr.Pin()->refreshDetails = true;
}

int32 SDialogueNodeWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void SDialogueNodeWidget::Tick(const FGeometry & AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	NodeSize = AllottedGeometry.GetLocalSize(); //TD: is it possible to bind it?
}

FReply SDialogueNodeWidget::OnMouseButtonDoubleClick(const FGeometry & InMyGeometry, const FPointerEvent & InMouseEvent)
{
	bJustDoubleClicked = true;
	return FReply::Unhandled();
}

void SDialogueNodeWidget::OnMouseEnter(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	// if mouse is over a node widget after we stopped linkingAndPanning, try to link the node
	if (Dialogue->m_bIsLinking && !Owner->isLinkingAndCapturing) 
	{
		// we shouldn't link a node to itself
		if (Id != Dialogue->m_AryData[Dialogue->m_nLinkingFromIndex].m_nID) 
		{
			// and we shouldn't link a node that's already linked
			if (!Dialogue->m_AryData[Dialogue->m_nLinkingFromIndex].m_AryLinks.Contains(Id))
			{
				const FScopedTransaction Transaction(LOCTEXT("AddLink", "Add Link"));
				Dialogue->Modify();

				Dialogue->m_AryData[Dialogue->m_nLinkingFromIndex].m_AryLinks.Add(Id);
				SortParentsLinks();
			}
		}
		// whatever node we entered, we stop linking
		Dialogue->m_bIsLinking = false; 
	}
}

const FSlateBrush* SDialogueNodeWidget::GetNodeStyle() const
{	
	if (Id == 0) return FDialogueEditorStyle::Get()->GetBrush("StartNodeStyle");

	return Dialogue->m_AryData[NodeIndex].m_bIsPlayer ? FDialogueEditorStyle::Get()->GetBrush("PlayerNodeStyle") : FDialogueEditorStyle::Get()->GetBrush("NpcNodeStyle");
}

FText SDialogueNodeWidget::GetNodeText() const
{
	if (!isVisible)
	{
		return FText::FromString("");
	}

	FText NodeText = Dialogue->m_AryData[NodeIndex].m_Text;

	int maxTextLength = 300; //@TODO: change 300 to user's choice

	if (NodeText.ToString().Len() > maxTextLength)
	{
		FString Output;
		
		Output += NodeText.ToString();
		Output.RemoveAt(maxTextLength, NodeText.ToString().Len() - maxTextLength, true);
		Output += TEXT("...");

		return FText::FromString(Output);
	}
	
	return NodeText;
}

FText SDialogueNodeWidget::GetFieldText() const
{
	if (!isVisible)
	{
		return FText::FromString("");
	}

	return Dialogue->m_AryData[NodeIndex].m_Text;
}

FReply SDialogueNodeWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Handled();

	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		Reply.DetectDrag(SharedThis(this), EKeys::RightMouseButton);
	}
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		Reply.DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}

	return Reply;
}

FReply SDialogueNodeWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !Dialogue->m_bIsLinking)
	{
		Owner->isPanning = true;
	}
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (Owner->bBreakingLinksMode)
			Owner->bBreakingLinksMode = false;

		const FScopedTransaction Transaction(LOCTEXT("DragNodes", "Drag Nodes"));
		Dialogue->Modify();

		// offset = local coords of the center of the node - local coords of the mouse click (inside the node)
		Owner->draggingOffset = FVector2D(MyGeometry.GetLocalSize().X / 2, MyGeometry.GetLocalSize().Y / 2) - MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		Owner->StartDraggingIndex(NodeIndex);
	}
	return FReply::Handled();
}

FReply SDialogueNodeWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	/* Left mouse button UP */
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{	
		if ((bJustDoubleClicked || (Owner->SelectedNodes.Num() == 1 && Owner->SelectedNodes.Contains(Id) && NodeTextBlock->IsHovered())) && Id != 0)
		{
			bJustDoubleClicked = false;

			// Don't enter Edit mode if we're at -6 zoom or lower.
			if (Owner->GetZoomLevel() > 6)
			{
				NodeField->SetText(GetFieldText());
				Owner->EditTextNode = Id;
				FSlateApplication::Get().SetKeyboardFocus(NodeField);
			}

			return FReply::Handled();
		}

		if (Owner->bBreakingLinksMode)
		{
			// Breaking links between Id and Owner->breakingLinksFromId
			BreakLinksWithNode();
			return FReply::Handled();
		}

		return FReply::Unhandled();
	}
	/* Right mouse button UP */
	else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (Owner->bBreakingLinksMode)
		{
			Owner->bBreakingLinksMode = false;
		}

		if (!Owner->SelectedNodes.Contains(Id))
		{
			Owner->SelectNodes(Id);
			Dialogue->m_nCurrentNodeId = Id;
			Owner->ForceRefresh();
		}

		if (Dialogue->m_bIsLinking) // if RMB UP on a node when linking, cancel linking
		{
			Dialogue->m_bIsLinking = false;
			return FReply::Handled();
		}

		FMenuBuilder MenuBuilder(true, NULL);

		FUIAction AddLinkAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::OnAddLink));
		FUIAction AddPCAnswerAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::OnAddPcAnswer));
		FUIAction AddNPCAnswerAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::OnAddNpcAnswer));
		FUIAction BreakOutLinksAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::OnBreakOutLinks));
		FUIAction BreakInLinksAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::OnBreakInLinks));
		FUIAction ChangePcNpcAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::OnChangePcNpc));
		FUIAction DeleteAction(FExecuteAction::CreateSP(Owner, &SDialogueViewportWidget::DeleteOneNode, Id));
		FUIAction DeleteAllAction(FExecuteAction::CreateSP(Owner, &SDialogueViewportWidget::DeleteSelected));
		FUIAction BreakLinksModeAction(FExecuteAction::CreateSP(this, &SDialogueNodeWidget::BreakLinksMode));

		MenuBuilder.BeginSection(NAME_None, NSLOCTEXT("PropertyView", "ExpansionHeading", "Dialogue"));
		
		// can't give a PC answer to a previous PC node
		if (!Dialogue->m_AryData[NodeIndex].m_bIsPlayer)
		{
			MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "AddPcAnswer", "Add PC Answer"), NSLOCTEXT("PropertyView", "AddPcAnswer_ToolTip", "Adds a child node (PC Answer)"), FSlateIcon(), AddPCAnswerAction);
		}

		MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "AddNpcAnswer", "Add NPC Answer"), NSLOCTEXT("PropertyView", "AddNpcAnswer_ToolTip", "Adds a child node (NPC Answer)"), FSlateIcon(), AddNPCAnswerAction);
		MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "AddLink", "Add Link"), NSLOCTEXT("PropertyView", "AddLink_ToolTip", "Usage: left click on the node you want to link"), FSlateIcon(), AddLinkAction);

		if (Owner->SelectedNodes.Num() == 1 && Owner->SelectedNodes[0] == Id)
		{
			MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "BreakLinks", "Break Links With Node..."), NSLOCTEXT("PropertyView", "BreakLinks_ToolTip", "Usage: left click on the node you want to break links with"), FSlateIcon(), BreakLinksModeAction);
		}

		MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "BreakOutLinks", "Break Outgoing Links"), NSLOCTEXT("PropertyView", "BreakOutLinks_ToolTip", "Breaks all outgoing links"), FSlateIcon(), BreakOutLinksAction);
		MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "BreakInLinks", "Break Incoming Links"), NSLOCTEXT("PropertyView", "BreakInLinks_ToolTip", "Breaks all incoming links"), FSlateIcon(), BreakInLinksAction);

		if (Id != 0)
		{
			if (Dialogue->m_AryData[NodeIndex].m_bIsPlayer)
			{
				MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "ChangeToNpc", "Change to NPC"), NSLOCTEXT("PropertyView", "ChangeToNpc_ToolTip", "Changes node to an NPC answer"), FSlateIcon(), ChangePcNpcAction);
			}
			else
			{
				MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "ChangeToPc", "Change to PC"), NSLOCTEXT("PropertyView", "ChangeToPc_ToolTip", "Changes node to a PC answer"), FSlateIcon(), ChangePcNpcAction);
			}
		}

		//can't delete start node
		if (Id != 0)	
		{
			MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "Delete", "Delete Node"), NSLOCTEXT("PropertyView", "Delete_ToolTip", "Deletes this node"), FSlateIcon(), DeleteAction);
		}

		if (Owner->SelectedNodes.IsValidIndex(0) && Owner->SelectedNodes.IsValidIndex(1)) // if multiple are selected
		{
			MenuBuilder.AddMenuEntry(NSLOCTEXT("PropertyView", "DeleteSelected", "Delete Selected Nodes"), NSLOCTEXT("PropertyView", "DeleteAll_ToolTip", "Deletes selected nodes"), FSlateIcon(), DeleteAllAction);
		}

		MenuBuilder.EndSection();

		FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
		FSlateApplication::Get().PushMenu(AsShared(), WidgetPath, MenuBuilder.MakeWidget(), MouseEvent.GetScreenSpacePosition(), FPopupTransitionEffect::ContextMenu);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void SDialogueNodeWidget::BreakLinksMode()
{
	Owner->bBreakingLinksMode = true;
	Owner->breakingLinksFromId = Id;
}

void SDialogueNodeWidget::BreakLinksWithNode()
{
	Owner->bBreakingLinksMode = false;

	// if the two nodes aren't linked in any way or we clicked on the same node, just return
	if (Owner->breakingLinksFromId == Id || (!Dialogue->m_AryData[NodeIndex].m_AryLinks.Contains(Owner->breakingLinksFromId) && !Owner->GetNodeById(Owner->breakingLinksFromId).m_AryLinks.Contains(Id)))
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("BreakLinksWithNode", "Break Links With Node..."));
	Dialogue->Modify();

	Dialogue->m_AryData[NodeIndex].m_AryLinks.RemoveAll([this](const int32 param1)
	{
		return (param1 == Owner->breakingLinksFromId);
	});

	int32 BreakingFromNodeIndex = Owner->NodeIdsIndexes.FindRef(Owner->breakingLinksFromId);

	Dialogue->m_AryData[BreakingFromNodeIndex].m_AryLinks.RemoveAll([this](const int32 param1)
	{
		return (param1 == Id);
	});
}

void SDialogueNodeWidget::OnChangePcNpc()
{
	if (Dialogue->m_AryData[NodeIndex].m_bIsPlayer)
	{
		const FScopedTransaction Transaction(LOCTEXT("ChangePcNpc", "Switch Node To NPC"));
		Dialogue->Modify();
	}
	else
	{
		const FScopedTransaction Transaction(LOCTEXT("ChangeNpcPc", "Switch Node To PC"));
		Dialogue->Modify();
	}

	Dialogue->m_AryData[NodeIndex].m_bIsPlayer = !Dialogue->m_AryData[NodeIndex].m_bIsPlayer;
}

/* Called when deleting a single node.
* @param withRefresh - indicates wether a refresh is required after this node is deleted. You don't want to refresh when you're deleting multiple nodes with this method.
*/
void SDialogueNodeWidget::OnDeleteNode(bool withRefresh = true)
{
	if (Id == 0) return; //can't delete start node

	BreakInLinks();

	Dialogue->m_nCurrentNodeId = -1;
	Dialogue->m_AryData.RemoveAt(Owner->NodeIdsIndexes.FindRef(Id));
	Owner->DeselectNode(Id);

	if (withRefresh)
	{
		Owner->SpawnNodes();
	}
}

void SDialogueNodeWidget::OnBreakOutLinks()
{
	const FScopedTransaction Transaction(LOCTEXT("BreakOutLinks", "Break Outgoing Links"));
	Dialogue->Modify();

	Dialogue->m_AryData[NodeIndex].m_AryLinks.Empty();
}

void SDialogueNodeWidget::OnBreakInLinks()
{
	const FScopedTransaction Transaction(LOCTEXT("BreakInLinks", "Break Incoming Links"));
	Dialogue->Modify();
	BreakInLinks();
}

void SDialogueNodeWidget::BreakInLinks()
{
	int i = 0;
	for (auto Node : Dialogue->m_AryData)
	{
		Dialogue->m_AryData[i].m_AryLinks.RemoveAll([this](const int32 param1)
		{
			return (param1 == Id);
		});
		i++;
	}	
}

void SDialogueNodeWidget::OnAddPcAnswer()
{
	const FScopedTransaction Transaction(LOCTEXT("AddPcAnswer", "Add PC Answer"));
	Dialogue->Modify();

	int index = NodeIndex;

	FDialogueNode NewNode;

	NewNode.m_nID = Dialogue->m_nNextNodeId;
	NewNode.m_bIsPlayer = true;

	if (Dialogue->m_AryData[index].m_AryLinks.Num() == 0)
	{
		NewNode.m_Coordinates.X = Dialogue->m_AryData[index].m_Coordinates.X;
		NewNode.m_Coordinates.Y = Dialogue->m_AryData[index].m_Coordinates.Y + (NodeSize.Y + Owner->NodeWidgets[0]->NodeSize.Y) / 2.f / Owner->GetZoomAmount() + 75.f; // half the size of current widget + half the size of first widget + desired distance
	}
	else
	{
		int32 lastIndex;
		int32 lastId;
		int32 linkToLastId = Dialogue->m_AryData[index].m_AryLinks.FindLastByPredicate([&](const int32 param1)
		{
			int tempIndex = Owner->NodeIdsIndexes.FindRef(param1);
			return Dialogue->m_AryData[tempIndex].m_Coordinates.Y > Dialogue->m_AryData[index].m_Coordinates.Y;
		});

		if (linkToLastId != INDEX_NONE)
		{
			lastId = Dialogue->m_AryData[index].m_AryLinks[linkToLastId];
			lastIndex = Owner->NodeIdsIndexes.FindRef(lastId);

			NewNode.m_Coordinates.X = Dialogue->m_AryData[lastIndex].m_Coordinates.X + (Owner->NodeWidgets[lastIndex]->NodeSize.X + Owner->NodeWidgets[0]->NodeSize.X) / 2.f / Owner->GetZoomAmount() + 50.f; // half the size of the preceding widget + half the size of the new widget + desired distance
			NewNode.m_Coordinates.Y = Dialogue->m_AryData[lastIndex].m_Coordinates.Y;
		}
		else
		{	// same as if (Dialogue->Data[index].Links.Num() == 0)
			NewNode.m_Coordinates.X = Dialogue->m_AryData[index].m_Coordinates.X;
			NewNode.m_Coordinates.Y = Dialogue->m_AryData[index].m_Coordinates.Y + (NodeSize.Y + Owner->NodeWidgets[0]->NodeSize.Y) / 2.f / Owner->GetZoomAmount() + 75.f; // half the size of current widget + half the size of first widget + desired distance
		}
	}

	Dialogue->m_nNextNodeId++;
	Dialogue->m_AryData.Add(NewNode);
	Dialogue->m_AryData[index].m_AryLinks.Add(NewNode.m_nID);
	Owner->SpawnNodes(NewNode.m_nID);
}

void SDialogueNodeWidget::OnAddNpcAnswer()
{
	const FScopedTransaction Transaction(LOCTEXT("AddNpcAnswer", "Add NPC Answer"));
	Dialogue->Modify();

	int index = NodeIndex;

	FDialogueNode NewNode;

	NewNode.m_nID = Dialogue->m_nNextNodeId;
	NewNode.m_bIsPlayer = false;

	if (Dialogue->m_AryData[index].m_AryLinks.Num() == 0)
	{
		NewNode.m_Coordinates.X = Dialogue->m_AryData[index].m_Coordinates.X;
		NewNode.m_Coordinates.Y = Dialogue->m_AryData[index].m_Coordinates.Y + (NodeSize.Y + Owner->NodeWidgets[0]->NodeSize.Y) / 2.f / Owner->GetZoomAmount() + 75.f; // half the size of current widget + half the size of first widget + desired distance
	}
	else
	{
		int32 lastIndex;
		int32 lastId; 
		int32 linkToLastId = Dialogue->m_AryData[index].m_AryLinks.FindLastByPredicate([&](const int32 param1)
		{
			int tempIndex = Owner->NodeIdsIndexes.FindRef(param1);
			return Dialogue->m_AryData[tempIndex].m_Coordinates.Y > Dialogue->m_AryData[index].m_Coordinates.Y;
		});

		if (linkToLastId != INDEX_NONE)
		{
			lastId = Dialogue->m_AryData[index].m_AryLinks[linkToLastId];
			lastIndex = Owner->NodeIdsIndexes.FindRef(lastId);

			NewNode.m_Coordinates.X = Dialogue->m_AryData[lastIndex].m_Coordinates.X + (Owner->NodeWidgets[lastIndex]->NodeSize.X + Owner->NodeWidgets[0]->NodeSize.X) / 2.f / Owner->GetZoomAmount() + 50.f; // half the size of the preceding widget + half the size of the new widget + desired distance
			NewNode.m_Coordinates.Y = Dialogue->m_AryData[lastIndex].m_Coordinates.Y;
		}
		else
		{	// same as if (Dialogue->Data[index].Links.Num() == 0)
			NewNode.m_Coordinates.X = Dialogue->m_AryData[index].m_Coordinates.X;
			NewNode.m_Coordinates.Y = Dialogue->m_AryData[index].m_Coordinates.Y + (NodeSize.Y + Owner->NodeWidgets[0]->NodeSize.Y) / 2.f / Owner->GetZoomAmount() + 75.f; // half the size of current widget + half the size of first widget + desired distance
		}
	}

	Dialogue->m_nNextNodeId++;
	Dialogue->m_AryData.Add(NewNode);
	Dialogue->m_AryData[index].m_AryLinks.Add(NewNode.m_nID);
	Owner->SpawnNodes(NewNode.m_nID);
}

void SDialogueNodeWidget::OnAddLink()
{
	Dialogue->m_bIsLinking = true;
	Owner->isLinkingAndCapturing = true;
	Dialogue->m_nLinkingFromIndex = NodeIndex;
	Dialogue->m_LinkingCoords = Dialogue->m_AryData[NodeIndex].m_Coordinates;
	Owner->ForceSlateToStayAwake();
}

/* After moving a node, we have to re-sort its parents child-links, so that the replies are shown based on the nodes horizontal positionning.
* For example, if Node A is to the left of Node B, then in-game you will see the answer A before answer B.
*/
void SDialogueNodeWidget::SortParentsLinks()
{
	int32 i = 0;
	for (auto Node : Dialogue->m_AryData)
	{
		int32 j = 0;
		for (auto Link : Node.m_AryLinks)
		{
			if (Dialogue->m_AryData[i].m_AryLinks[j] == Id) // if a link to current node has been found, then perform a sort on the links
			{
				Dialogue->m_AryData[i].m_AryLinks.Sort([&](const int32 id1, const int32 id2) // resort them depending on their x coordinate
				{
					int32 index1 = Owner->NodeIdsIndexes.FindRef(id1);
					int32 index2 = Owner->NodeIdsIndexes.FindRef(id2);

					return Dialogue->m_AryData[index1].m_Coordinates.X < Dialogue->m_AryData[index2].m_Coordinates.X;
				});
				break;
			}
			j++;
		}
		i++;
	}
}

#undef LOCTEXT_NAMESPACE
