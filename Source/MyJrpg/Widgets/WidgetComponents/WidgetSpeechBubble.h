// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "WidgetSpeechBubble.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetSpeechBubble : public UUserWidget
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE(FOnComplete);
	
	FOnComplete m_OnComplete;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextText;

protected:
	FString m_TypingWantString;

	FString m_BlendingString;

	int m_nTypeRepeatCounter;

	int m_nStringLength;

	FTimerHandle m_TypeTimerHandle;

	FTimerHandle m_HideTimerHandle;
	
protected:
	void RunStringTypingTimer(FString originalString, float charDelay);

	void RunStringTyping();

	void CompleteTyping();

	void Hide();
	
public:
	void Speech(const FText& text);
	
	void SetText(const FText& text);
};

