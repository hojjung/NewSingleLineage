// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "TextText.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "WidgetLevelHealthInfo.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetLevelHealthInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_HpBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	//m_HpBar
protected:
	virtual void NativeOnInitialized() override;

	void UpdateLevel();

	void UpdateHp(const FStatGroup& stat);
};
