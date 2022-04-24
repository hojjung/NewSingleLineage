// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WorldWidgetStruct.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWorldWidgetStruct : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRotation;
protected:
	virtual void NativeOnInitialized() override;
	
public:
	void ShowRotation(bool b);

	void ShowBuildWidget(bool b);

public:
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnRotation();
};
