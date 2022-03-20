// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "WidgetChatElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetChatElement : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* m_TextChat;
	
public:
	void SetChat(const FString& chat);
	
	void Clear();

	static FString GetRankColor(const FString& rankNumber);

	static FString GetRankColor(int rankNumber);
};
