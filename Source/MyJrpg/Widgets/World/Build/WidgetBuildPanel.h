#pragma once

#include "CoreMinimal.h"
#include "WidgetBuildElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/CharacterInfoHUD/WidgetBuffElement.h"
#include "WidgetBuildPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetBuildPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetBuildElement> m_ClassBuildEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollElements;
	UPROPERTY()
	UWidgetBuildElement* m_Focused;
	
protected:
	virtual void NativeOnInitialized() override;

	void OnClickElement(UWidgetBuildElement* ele,const FBuildDataRow& data);
	
public:
	virtual void OpenPanel() override;

	virtual void ClosePanel() override;
};
