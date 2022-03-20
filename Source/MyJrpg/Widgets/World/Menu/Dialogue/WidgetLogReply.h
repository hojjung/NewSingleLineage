// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dialogue.h"
#include "Components/CanvasPanel.h"
#include "Components/RichTextBlock.h"
#include "WidgetLogReply.generated.h"


class UWidgetLogWindow;

UCLASS()
class MYJRPG_API UWidgetLogReply : public UUserWidget
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClick,const FDialogueNode&);

	FOnClick m_OnClick;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_SpeakerPortrait;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* m_TextReply;
	
	const FDialogueNode* m_ReplyNode;

public:
	void Init(const FDialogueNode& node,UWidgetLogWindow* log);

	FReply OnClick(const FPointerEvent& inputEvent);

public:
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
