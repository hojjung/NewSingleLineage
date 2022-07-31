// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetConfirm.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetConfirmPanel.h"
#include "MyJrpg/Widgets/World/Menu/CharacterInfoHUD/AlertInfoWindow.h"

#include "WidgetCanvasInit.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCanvasInit : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgBlink;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UAlertInfoWindow* m_AlertInfoWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetConfirm* m_WidgetConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_NewsCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTitleNews;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCloseNews;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetConfirmPanel* m_ConfirmPanel;
	float m_fTimer;
public:
	void PrintInfoText(const FString& str, FLinearColor color);
	
	UWidgetConfirmPanel* GetConfirmPanel();

protected:
	void OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt);

	void OpenConfirmPanel();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UFUNCTION()
	void OnCloseNews();

	void StartPlayfabLogin();
};

