#include "WidgetQuickSlotEle.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetQuickSlotEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nIndex = INDEX_NONE;
	
	Clear();
}

FReply UWidgetQuickSlotEle::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	m_OnClick.Broadcast(this);
	
	return FReply::Unhandled();
}

void UWidgetQuickSlotEle::Clear()
{
	m_ImgItemIcon->SetVisibility(ESlateVisibility::Collapsed);

	HideTextStackLv();

	m_ItemData = nullptr;
}

UWidget* UWidgetQuickSlotEle::GetImgIcon()
{
	return m_ImgItemIcon->GetParent();
}

void UWidgetQuickSlotEle::SetIcon(TSoftObjectPtr<UTexture2D> t)
{
	m_ImgItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ImgItemIcon->SetIcon(t.LoadSynchronous());
}

void UWidgetQuickSlotEle::SetTextStackLv(FString t)
{
	SetTextStackLv(FText::FromString(t));
}

void UWidgetQuickSlotEle::SetTextStackLv(FText t)
{
	m_TextStackAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextStackAmount->SetText(t);
}

void UWidgetQuickSlotEle::HideTextStackLv()
{
	m_TextStackAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetQuickSlotEle::SetIndex(int index)
{
	m_nIndex = index;
}

int UWidgetQuickSlotEle::GetIndex()
{
	return m_nIndex;
}

void UWidgetQuickSlotEle::SetBoxSize(float x, float y)
{
	m_Size->WidthOverride = x;

	m_Size->HeightOverride = y;
}

void UWidgetQuickSlotEle::SetItem(const FItemSpec& itemSpec)
{
	const FItemDataRow& Data = UMyLib::GetItemData(itemSpec.m_ID);

	SetItemData(Data);

	bool IsEquip = UMyLib::IsEquip(Data);

	if(IsEquip)
	{
		if(itemSpec.m_nLvStack > 0)
		{
			FString Str = FString::Printf(TEXT("+%d"), itemSpec.m_nLvStack);
			
			SetTextStackLv(Str);
		}
		else
		{
			HideTextStackLv();
		}
	}
	else
	{
		FString Str = FString::Printf(TEXT("%d"), itemSpec.m_nLvStack);
			
		SetTextStackLv(Str);
	}
}

void UWidgetQuickSlotEle::SetItemData(const FItemDataRow& itemData)
{
	m_ItemData = &itemData;
	
	SetIcon(m_ItemData->m_Icon);
}

const FItemDataRow* UWidgetQuickSlotEle::GetItemData() const
{
	return m_ItemData;
}
