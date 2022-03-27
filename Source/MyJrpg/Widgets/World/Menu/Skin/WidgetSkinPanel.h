// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSkinElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetPreview.h"
#include "WidgetSkinPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetSkinPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetPreview* m_Preview;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_Wrap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetSkinElement> m_ClassElement;
	UPROPERTY()
	TArray<UWidgetSkinElement*> m_AryEles;
	UPROPERTY()
	UWidgetSkinElement* m_CurrentFocused;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateElements();

	void OnSelected(UWidgetSkinElement* ele);

public:
	void Open();
	UFUNCTION()
	void OnClose();
	
};
