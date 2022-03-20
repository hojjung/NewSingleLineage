// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

#include "WidgetSpeechBubbleComp.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetSpeechBubbleComp : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UWidgetSpeechBubbleComp();

protected:
	void Hide();

	virtual void BeginPlay() override;
public:
	void Speech(const FText& text);
};
