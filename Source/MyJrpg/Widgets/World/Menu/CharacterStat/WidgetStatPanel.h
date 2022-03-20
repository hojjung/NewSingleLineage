// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "TextText.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "WidgetStatPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetStatPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollBarStat;
	UPROPERTY()
	TArray<UWidgetTextText*> m_AryText;
	
private:
	void CreateText(int max);
	
protected:
	void UpdateStats();
	
	virtual void NativeOnInitialized() override;

	void AddStatText(FText statName, float v, int index);

	void AddStatText(FText statName, FText statValue, int index);
};
