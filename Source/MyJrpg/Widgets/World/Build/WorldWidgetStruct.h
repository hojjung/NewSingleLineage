// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "WorldWidgetStruct.generated.h"

class AStructureActor;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnErase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayErase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY()
	AStructureActor* m_Owner;
protected:
	virtual void NativeOnInitialized() override;

	void SelectErase();

	void DeselectErase();

	void ConfirmErase();
	
public:
	void ShowRotation(bool b);

	void ShowBuildWidget(bool b);

	void ShowSelect(bool b);

	void SetOwnerActor(AStructureActor* actor);
	
public:
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnRotation();
	UFUNCTION()
	void OnErase();
	UFUNCTION()
	void OnUpgrade();
};
