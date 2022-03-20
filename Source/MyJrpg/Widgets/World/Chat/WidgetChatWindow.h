// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetChatElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "WidgetChatWindow.generated.h"
UCLASS()
class MYJRPG_API UWidgetChatWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_ChatTotalWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_MesageEditableText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_MessageScrollBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_SendButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_ChatWindowToggle;
	UPROPERTY()
	TArray<UWidgetChatElement*> m_AryChatText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetChatElement> m_ClassTextWidget;

	int m_nTopIndex;

	float m_fMaxOffset;

	bool m_bIsOpened;
	
protected:
	UWidgetChatElement* GetTopText();
	
	virtual void NativeOnInitialized() override;

	void ClearChat();
	
public:
	UFUNCTION()
	void OnReceiveTotalChatList(const FString& chat);
	
	UFUNCTION()
	void SendText();

	UFUNCTION()
	void AddTextWidget(const FString& chat);

	UFUNCTION()
	void OnChatTextChanged(const FText& text);

	UFUNCTION()
	void SetChatWindowOpenClose();
};
