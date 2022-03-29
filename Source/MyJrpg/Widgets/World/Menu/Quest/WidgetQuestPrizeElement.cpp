// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetQuestPrizeElement.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetQuestPrizeElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_QuestBaseEle->SetFocusable(false);

	m_QuestBaseEle->SetHoldable(true);

	m_QuestBaseEle->m_OnHold.AddUObject(this,&UWidgetQuestPrizeElement::OnHoldComplete);
}

void UWidgetQuestPrizeElement::OnHoldComplete()
{
	FName ID = m_QuestReward.m_Item.RowName;

	if (!ID.IsNone())
		UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::QuestReward,ID,nullptr);
}

void UWidgetQuestPrizeElement::SetPrize(const FQuestReward& quest_reward)
{
	m_QuestReward = quest_reward;

	UpdatePrize();
}

void UWidgetQuestPrizeElement::UpdatePrize()
{
	TSoftObjectPtr<UTexture2D> Icon = nullptr;
	
	if(m_QuestReward.m_Currency.RowName != NAME_None)
	{
		//Reward Is Currency
		Icon = m_QuestReward.m_Currency.GetRow<FCurrencyDataRow>("")->m_Icon;

		m_TextAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		if(m_QuestReward.m_Currency.RowName==TEXT("Exp"))
		{
			float Amount = m_QuestReward.m_nAmount / UMyGameInstance::Get->m_PlayerStatManager->GetMaxExp();

			Amount*=100.0f;

			FString Str = FString::Printf(TEXT("%.2f%%"),Amount);

			m_TextAmount->SetText(FText::FromString(Str));
		}
		else
		{
			m_TextAmount->SetText(FText::AsNumber(m_QuestReward.m_nAmount));	
		}
	}
	else if(m_QuestReward.m_Item.RowName != NAME_None)
	{
		const FItemDataRow& ItemRow = *m_QuestReward.m_Item.GetRow<FItemDataRow>("");
		
		Icon = m_QuestReward.m_Item.GetRow<FItemDataRow>("")->m_ItemIcon;

		if(UMyLib::GetItemType(ItemRow) == EItemType::Equip)
		{
			m_TextAmount->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			m_TextAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			m_TextAmount->SetText(FText::AsNumber(m_QuestReward.m_nAmount));
		}
	}

	m_QuestBaseEle->SetIcon(Icon);
}
