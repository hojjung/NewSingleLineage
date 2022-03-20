// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetZoneSelectButton.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"

#include "WidgetZoneSelectPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetZoneSelectPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWorldMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSpecialMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBossMap;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidgetZoneSelectButton> m_ClassButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollWorldButton;//Test
	
protected:
	virtual void NativeOnInitialized() override;
	
	void CreateButtons();
};
