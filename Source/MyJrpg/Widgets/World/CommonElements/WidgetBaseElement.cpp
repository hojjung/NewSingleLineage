#include "WidgetBaseElement.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"

void UWidgetBaseElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bIsHoldable = true;

	m_bIsFocusable = true;

	m_bIsDragable = true;

	m_nIndex = INDEX_NONE;
	
	Clear();

	SetMyUnFocus();
	
	HideDurBar();
}

FReply UWidgetBaseElement::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	if(!m_bIsHoldable)
	{
		return FReply::Unhandled();
	}
	
	m_bHolding = true;

	m_fTimer = 0;
	
	return FReply::Unhandled();
}

FReply UWidgetBaseElement::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if (InGestureEvent.GetCursorDelta().SizeSquared()>50.0f)
	{
		EndHolding();

		if(!UItemDDO::GetDDOInst &&  m_bIsDragable)
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InGestureEvent,this,EKeys::LeftMouseButton).NativeReply;
		}
	}

	return FReply::Handled();
}

FReply UWidgetBaseElement::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	EndHolding();
	
	if(m_bIsFocusable && m_fTimer < 0.7f)
	{
		SetMyFocus();
		
		m_OnFocus.Broadcast(this);
	}

	return FReply::Handled();
}

void UWidgetBaseElement::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	EndHolding();
}

void UWidgetBaseElement::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	OutOperation = CreateDDO();
	m_OnDrag.Broadcast(this);
}

UDragDropOperation* UWidgetBaseElement::CreateDDO()
{
	UItemDDO* DDO = Cast<UItemDDO>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDDO::StaticClass()));
	DDO->SetDDO(this);
	return DDO;
}

bool UWidgetBaseElement::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool Result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if(UItemDDO::GetDDOInst != InOperation)
	{
		return false;
	}

	if(UItemDDO::GetDDOInst->DefaultDragVisual == GetImgIcon())
	{
		return false;
	}
	
	m_OnDrop.Broadcast(this);

	return true;
}

void UWidgetBaseElement::HideDurBar()
{
	m_DurParent->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBaseElement::ShowDurBar(float per)
{
	m_DurParent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Dur->SetPercent(per);
}

void UWidgetBaseElement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!m_bHolding ||!m_bIsHoldable || UItemDDO::GetDDOInst) //포커싱이 이미되버렸으면 안함//||m_OverlayFocus->IsVisible()
	{
		return;
	}

	m_fTimer += InDeltaTime;

	if (m_fTimer >= 0.2f)
	{
		m_HoldingBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (m_fTimer >= 0.7f)
	{
		m_OnHold.Broadcast(this);

		EndHolding();
	}

	m_HoldingBar->SetPercent(m_fTimer / 0.7f);
}

void UWidgetBaseElement::Clear()
{
	EndHolding();
	
	m_ImgGlow->SetBrushFromTexture(m_DefaultGlow);
	
	m_ImgItemIcon->SetVisibility(ESlateVisibility::Collapsed);

	HideTextStackLv();

	SetFocusable(false);
	SetDragable(false);
	SetHoldable(false);

	HideDurBar();
}

void UWidgetBaseElement::SetHoldable(bool isActive)
{
	m_bIsHoldable = isActive;
}
//
void UWidgetBaseElement::EndHolding()
{
	m_bHolding = false;

	m_HoldingBar->SetVisibility(ESlateVisibility::Collapsed);
}

UWidget* UWidgetBaseElement::GetImgIcon()
{
	return m_ImgItemIcon->GetParent();
}

void UWidgetBaseElement::SetIcon(TSoftObjectPtr<UTexture2D> t)
{
	m_ImgItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ImgItemIcon->SetBrushFromSoftTexture(t);
}

void UWidgetBaseElement::SetGlowColor(const FColorDataHandle& color)
{
	const FColorDataRow* ColorDataRow = color.GetRow<FColorDataRow>("");
	
	m_ImgGlow->SetBrushFromTexture(ColorDataRow->m_GlowTexture);
}

void UWidgetBaseElement::SetMyFocus()
{
	m_OverlayFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetBaseElement::SetMyUnFocus()
{
	m_OverlayFocus->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBaseElement::SetTextStackLv(FString t)
{
	SetTextStackLv(FText::FromString(t));
}

void UWidgetBaseElement::SetTextStackLv(FText t)
{
	m_TextStackAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextStackAmount->SetText(t);
}

void UWidgetBaseElement::HideTextStackLv()
{
	m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBaseElement::SetIndex(int index)
{
	m_nIndex = index;
}

int UWidgetBaseElement::GetIndex()
{
	return m_nIndex;
}

void UWidgetBaseElement::SetTextFocus(FText t)
{
	m_TextFocus->SetText(t);
}

void UWidgetBaseElement::SetFocusable(bool isActive)
{
	m_bIsFocusable = isActive;
}

void UWidgetBaseElement::SetDragable(bool isActive)
{
	m_bIsDragable = isActive;
}

bool UWidgetBaseElement::IsMyFocused() const
{
	return m_OverlayFocus->IsVisible();
}
