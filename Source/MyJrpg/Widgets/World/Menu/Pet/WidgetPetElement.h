// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Managers/PetManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetPetElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPetElement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClicked, UWidgetPetElement*);

	FOnClicked m_OnFocus;

protected:
	TWeakObjectPtr<UPetManager> m_PetManager;

	const FPetRow* m_Pet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Ele;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_FocusParent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextFocus;
	 
public:
	void Init(const FPetRow& pl);
	
	void UpdateEle();
	
	void SetMyUnfocus();
	
	void SetMyFocus();
	
	void SetConfirm();

	const FPetRow& GetCrntPet();

protected:
	void OnClicked();
};
