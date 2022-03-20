#include "WidgetSpeechBubble.h"

#include "TimerManager.h"

void UWidgetSpeechBubble::Speech(const FText& text)
{
	GetWorld()->GetTimerManager().ClearTimer(m_HideTimerHandle);
	
	RunStringTypingTimer(text.ToString(),0.045f);
}

void UWidgetSpeechBubble::RunStringTypingTimer(FString originalString, float charDelay)
{
	if (originalString.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Empty String"));
		return;
	}

	if (GetWorld()->GetTimerManager().GetTimerRemaining(m_TypeTimerHandle) != -1)
	{
		CompleteTyping();
	}

	m_TextText->SetText(FText());

	m_BlendingString.Reset();
	m_nTypeRepeatCounter = 0;
	
	m_TypingWantString = originalString;
	m_nStringLength = m_TypingWantString.Len();

	GetWorld()->GetTimerManager().SetTimer(m_TypeTimerHandle, this, &UWidgetSpeechBubble::RunStringTyping, charDelay,originalString.Len() > 0);
}

void UWidgetSpeechBubble::RunStringTyping()
{
	m_BlendingString += m_TypingWantString[m_nTypeRepeatCounter];

	m_nTypeRepeatCounter++;

	if (m_nTypeRepeatCounter >= m_nStringLength)
	{
		CompleteTyping();
	}
	else
	{
		m_TextText->SetText(FText::FromString(m_BlendingString));
	}
}

void UWidgetSpeechBubble::CompleteTyping()
{
	GetWorld()->GetTimerManager().ClearTimer(m_TypeTimerHandle);

	m_TextText->SetText(FText::FromString(m_TypingWantString));

	GetWorld()->GetTimerManager().SetTimer(m_HideTimerHandle, this, &UWidgetSpeechBubble::Hide, 2,
										false);
}

void UWidgetSpeechBubble::Hide()
{
	m_TextText->SetText(FText());
	
	m_OnComplete.ExecuteIfBound();
}




