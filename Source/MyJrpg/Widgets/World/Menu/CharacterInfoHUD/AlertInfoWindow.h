#pragma once

#include "CoreMinimal.h"
#include "AlertInfoElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "AlertInfoWindow.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UAlertInfoWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAlertInfoElement> m_ClassInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayInfo;
	UPROPERTY()
	TArray<UAlertInfoElement*> m_AryInfo;
	UPROPERTY()
	int m_nIndex;
	
protected:
	virtual void NativeOnInitialized() override;

	UAlertInfoElement* GetTextElement();
	
public:
	void PrintInfoText(const FString& reason);
};
