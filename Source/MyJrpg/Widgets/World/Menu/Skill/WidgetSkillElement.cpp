#include "WidgetSkillElement.h"

FReply UWidgetSkillElement::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	//m_SkillEle->NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetSkillElement::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//m_SkillEle->NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

FReply UWidgetSkillElement::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	//m_SkillEle->NativeOnTouchMoved(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetSkillElement::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	//m_SkillEle->NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}

void UWidgetSkillElement::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	//m_SkillEle->NativeOnMouseLeave(InMouseEvent);
}

void UWidgetSkillElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Locked->SetVisibility(ESlateVisibility::Collapsed);

	m_SkillEle->SetHoldable(true);

	m_SkillEle->SetFocusable(true);

	//m_SkillEle->m_OnClick.AddUObject(this,&UWidgetSkillElement::OnFocus);
	
	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkillElement::OnFocus()
{
	m_OnFocus.Broadcast(this);
	
	m_SkillEle->SetMyFocus();
	
	m_TextFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextFocus->SetText(NSLOCTEXT("UWidgetSkillElement","OnFocus","Equip?"));
}

void UWidgetSkillElement::SetSkill(FName id, bool isUnlock)
{
	if(isUnlock)
	{
		m_Locked->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_Locked->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	m_SkillID = id;
	
	auto* SkillData =  USkillData::GetSkillTable->FindRow<FSkillDataRow>(m_SkillID,"");
	
	//m_SkillEle->SetIcon(SkillData->m_SkillIcon);

	if(SkillData->m_bIsPassive)
	{
		m_TextIsActivate->SetText(FText::FromString(TEXT("PASSIVE")));
	}
	else
	{
		m_TextIsActivate->SetText(FText::FromString(TEXT("ACTIVE")));
	}

	FSlateColor& Color = SkillData->m_TierColor.GetRow<FColorDataRow>("")->m_Color;
	
	m_TextSkillName->SetColorAndOpacity(Color); 
}

void UWidgetSkillElement::SetMyUnFocus()
{
	m_SkillEle->SetMyUnFocus();

	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
}
