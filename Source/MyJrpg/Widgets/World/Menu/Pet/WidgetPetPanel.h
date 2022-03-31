// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetPetElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetPreview.h"
#include "WidgetPetPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPetPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetPreview* m_Preview;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_Wrap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetPetElement> m_ClassElement;
	UPROPERTY()
	TArray<UWidgetPetElement*> m_AryEles;
	UPROPERTY()
	UWidgetPetElement* m_CurrentFocused;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateElements();

	void OnSelected(UWidgetPetElement* ele);

public:
	void Open();
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnEquip();
};
