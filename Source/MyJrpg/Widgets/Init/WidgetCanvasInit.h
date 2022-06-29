// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetConfirm.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
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
	UAlertInfoWindow* m_AlertInfoWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetConfirm* m_WidgetConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgBlinkText;
	UPROPERTY()
	float m_fTimer;
	
public:
	virtual void NativeOnInitialized() override;
	
	void PrintErrorText(const FString& str);

protected:
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void GameStart();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

