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
	TArray<UWidgetBuildElement*> m_AryEles; 
	
	TWeakObjectPtr<UWidgetBuildElement> m_Focused;

	TWeakObjectPtr<AMyPlayerPawn> m_Pl;

	FDelegateHandle m_DeleTouchWorld;

	FDelegateHandle m_DeleCancel;

	FDelegateHandle m_DeleFurniture;
	//
	FDelegateHandle m_DeleInven;

	FDelegateHandle m_DeleBag;
	
protected:
	virtual void NativeOnInitialized() override;

	void OnClickElement(UWidgetBuildElement* ele,const FBuildDataRow& data);

	void OnTouchWorld(const FHitResult& hit);

	void OnFlick(const FVector2D& delta);

	void OnCancel();

	void UpdateFurnitureTab();

	void UpdateElement();

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	
	
public:
	virtual void OpenPanel() override;

	virtual void ClosePanel() override;

public:
	UFUNCTION()
	void OnClickStruct();
	UFUNCTION()
	void OnClickFurniture();
};


