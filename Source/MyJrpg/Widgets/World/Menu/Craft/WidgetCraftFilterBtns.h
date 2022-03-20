// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "MyJrpg/MyJrpg.h"
#include "WidgetCraftFilterBtns.generated.h"

class UWidgetCraftPanel;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCraftFilterBtns : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnHead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnTorso;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnHand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNeckless;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEarRing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBelt;
	UPROPERTY()
	UButton* m_BtnSelected;
	
protected:
	EEquipSlotType m_EquipType;
	
protected:
	virtual void NativeOnInitialized() override;
	
	void OnBtnPressed(UButton* btn, EEquipSlotType type);
	
public:
	void RegisterFilter(UWidgetCraftPanel* craft);

	EEquipSlotType GetCrntEquipType();

	void ClearFilter();
	
public:
	UFUNCTION()
	void OnFilterWeapon();
	UFUNCTION()
	void OnFilterHead();
	UFUNCTION()
	void OnFilterTorso();
	UFUNCTION()
	void OnFilterHand();
	UFUNCTION()
	void OnFilterLeg();
	UFUNCTION()
	void OnFilterRing();
	UFUNCTION()
	void OnFilterNeckless();
	UFUNCTION()
	void OnFilterEarRing();
	UFUNCTION()
	void OnFilterBelt();
};


UCLASS()
class MYJRPG_API UWidgetCraftColorFilterBtns : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWhite;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGreen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBlue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurple;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnYellow;
	UPROPERTY()
	UButton* m_BtnSelected;
	FName m_Color;
protected:
	virtual void NativeOnInitialized() override;
	
	void OnBtnPressed(UButton* btn , FName&& c);
	
public:
	void ClearFilter();
	
	void RegisterFilter(UWidgetCraftPanel* craft);
	
	FName GetCrntColor();

public:
	UFUNCTION()
	void OnFilterWhite();
	UFUNCTION()
	void OnFilterGreen();
	UFUNCTION()
	void OnFilterBlue();
	UFUNCTION()
	void OnFilterRed();
	UFUNCTION()
	void OnFilterPurple();
	UFUNCTION()
	void OnFilterYellow();
};

