// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DialogueUserWidget.h"
#include "WidgetLogReply.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "MyJrpg/MyJrpg.h"
#include "WidgetLogWindow.generated.h"

/**
 * 카톡방식
 */
UCLASS()
class MYJRPG_API UWidgetLogWindow : public UDialogueUserWidget
{
	GENERATED_BODY()

public:
	FVoidVoid m_OnClose;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetLogReply> m_ClassWidgetLogReply;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollReply;
	UPROPERTY()
	TArray<UWidgetLogReply*> m_AryReplies;
	
	const FDialogueNode* m_SpeakerNode;

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeDestruct() override;
	
	virtual void CompleteTyping() override;

	void StartDialogueWrap();

	void ClearReplies();

	void CloseDialogueWrap();

	UWidgetLogReply* CreateSpeakerReply(const FDialogueNode& rr);

	void TryRunQuestEvent(const FDialogueNode& rply);
	
public:
	virtual void StartDialogue(AActor*,UDialogue*) override;

	void RenderNextDialogue(const FDialogueNode& playerRply);

	UFUNCTION()
	void CloseDialogue();
	UFUNCTION()
	void OnClickContinue();//
};
