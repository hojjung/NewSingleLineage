#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MyJrpg/MyJrpg.h"
#include "WidgetScreenEffect.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetScreenEffect : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgBlack;

protected:
	float m_fFadeOutTimer;

	float m_fMaxFadeOut;

	FVoidVoid m_OnFadeOutDone;

	bool IsFadeOut;

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void EndFade();

public:
	void ShowFadeOut(float t, const FVoidVoid& onFadeOutEnd);

	void HideFadeOut();
};
