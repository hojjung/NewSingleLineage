// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "WidgetStackCalculator.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetStackCalculator : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnNumberChanged,int)

	DECLARE_DELEGATE_RetVal(int,FOnGetMax);

	FOnNumberChanged m_OnNumberAccept;

	FOnGetMax m_OnGetMax;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_TextNumber;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMinus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPlus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn00;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn06;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn07;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn08;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_Btn09;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnErase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAccept;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPlus001;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPlus010;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPlus100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMax;

protected:
	UPROPERTY()
	FText m_LastEnteredNumber;
	UPROPERTY()
	int m_nNumber;
	
protected:
	virtual void NativeOnInitialized() override;

	void SetAppendNumber(int number);

	void SetTextNumber(int numberNew);

	int GetMaxAmount();
	
public:
	UFUNCTION()
	void OnTextChanged(const FText& text);
	
	UFUNCTION()
	void On00();
	UFUNCTION()
	void On01();
	UFUNCTION()
	void On02();
	UFUNCTION()
	void On03();
	UFUNCTION()
	void On04();
	UFUNCTION()
	void On05();
	UFUNCTION()
	void On06();
	UFUNCTION()
	void On07();
	UFUNCTION()
	void On08();
	UFUNCTION()
	void On09();
	UFUNCTION()
	void OnMinus();
	UFUNCTION()
	void OnPlus();
	UFUNCTION()
	void OnErase();
	
	UFUNCTION()
	void OnClearToZero();
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnAccpet();
	UFUNCTION()
	void OnPlus010();
	UFUNCTION()
	void OnPlus100();
	UFUNCTION()
	void OnMax();

	void Open(int initAmount);
};
