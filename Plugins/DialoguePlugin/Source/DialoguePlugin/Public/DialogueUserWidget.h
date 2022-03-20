// Copyright Underflow Studios 2017

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

#include "DialogueUserWidget.generated.h"

/**
* 
*/
UCLASS()
class DIALOGUEPLUGIN_API UDialogueUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	FTimerHandle m_TypeTimerHandle;

	FString m_TypingWantString;

	FString m_BlendingString;

	int m_nTypeRepeatCounter;

	int m_nStringLength;
	
	UPROPERTY()
	URichTextBlock* m_RichTextBlock;
	UPROPERTY()
	UTextBlock* m_TextBlock;
	UPROPERTY()
	AActor* NPCActor;
	UPROPERTY()
	UDialogue* InDialogue;
	
protected:
	void RunStringTyping();

	bool IsTypingCompleteEnable();
public:
	virtual void StartDialogue(AActor*,UDialogue*);
	
	bool CheckNodeConditionEnable(const FDialogueNode& Node);

	void RunEventsForNode(const FDialogueNode& Node);

	FString ReplaceVarStrings(const FText& InText);

	bool RunStringReplacer(FString originalString, FString& resultString);
	
	TArray<FString> FindVarStrings(FText inText);

	void RunStringTypingTimer(FString originalString, float charDelay, UTextLayoutWidget* targetRichTextBlock);

	virtual void CompleteTyping();

	void SetTextLayout(UTextLayoutWidget* tt);

	void SetText(FText tt);
};

