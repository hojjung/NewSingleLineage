// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "PlayFabClientDataModels.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "MyJrpg/MyJrpg.h"

#include "WidgetConfirm.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetConfirm : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_CheckUsage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_CheckPrivate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_CheckRefund;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_CheckJoke01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextUsage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextPrivate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRefund;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_TextInputNickname;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;

	FString m_URL_Usage;
	
	FString m_URL_Private;
	
	FString m_URL_Refund;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void CheckTextTouch(const FPointerEvent& eve);
	
public:
	void Show();
	
	UFUNCTION()
	void OnConfirm();
	
	UFUNCTION()
	void OnTextChanged(const FText& text);

	void OpenWebUsage();

	void OpenWebPrivate();

	void OpenWebRefund();
};
