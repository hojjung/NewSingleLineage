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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidgetBuildElement> m_ClassBuildEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollElements;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollFurnitureElements;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnStruct;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnFurniture;
	UPROPERTY()
	UWidgetBuildElement* m_Focused;

	FDelegateHandle m_DeleTouchWorld;

	FDelegateHandle m_DeleCancel;

	FDelegateHandle m_DeleFurniture;

protected:
	virtual void NativeOnInitialized() override;

	void OnClickElement(UWidgetBuildElement* ele,const FBuildDataRow& data);

	void OnTouchWorld(const FHitResult& hit);

	void OnCancel();

	void UpdateFurnitureTab();
	
public:
	virtual void OpenPanel() override;

	virtual void ClosePanel() override;

public:
	UFUNCTION()
	void OnClickStruct();
	UFUNCTION()
	void OnClickFurniture();
};


