// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Widgets/World/CommonElements/MaterialProgressBar.h"
#include "WidgetInteract.generated.h"

class AItemActor;
class AMonsterPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetInteract : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnInteract;//Main 2
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_DurGauge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAttack;//Main 3 //공격버튼은 유닛에게 언제나 있을수있음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSneak;//sub all
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAuto;//sub all
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgUseAuto;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_DefaultPunch;
protected:
	UPROPERTY()
	AMyPlayerPawn* m_Pl;
	
	bool m_bAutoToggle;

protected:
	virtual void NativeOnInitialized() override;
	
	void HideDur();

	void ShowDur(float per);

	void OnEquipChanged();
	
	void ShowInteract(IFocusable* focus);

public:
	UFUNCTION()
	void OnInteract();
	UFUNCTION()
	void OnAttack();
	UFUNCTION()
	void OnSneak();
	UFUNCTION()
	void OnAutoToggle();
};

