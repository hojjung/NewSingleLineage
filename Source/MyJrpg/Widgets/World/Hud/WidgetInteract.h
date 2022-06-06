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

public:
	static bool AutoToggle;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnObtain;//Main 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSteal;//Main 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnControl;//Main 2
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnTalk;//Main 3 //대화는 결국 서브다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_DurGauge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAttack;//Main 3 //공격버튼은 유닛에게 언제나 있을수있음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPickPocket;//sub 3 //main으로 변경,상대방의 뒤를 잡았을때만 //대화 //조작
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSneak;//sub all
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAuto;//sub all
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgUseAuto;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn="true"))
	UMaterialInterface* m_MatWant;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Transient)
	UMaterialInstanceDynamic* m_MatInst;
protected:
	UPROPERTY()
	AMyPlayerPawn* m_Pl;
	
	bool m_bHasFocus;	

protected:
	virtual void NativeOnInitialized() override;
	
	void ShowWidgetMonster(const AMonsterPawn* mob);

	void ShowWidgetItem(const AItemActor* item);
	
	void ShowWidgetProp();
	
	void HideAllBtns();

	bool IsInRange(IFocusable* focus);

	void HideDur();

	void ShowDur(float per);

	void OnEquipChanged();

public:
	void ShowInteract(IFocusable* focus);
	UFUNCTION()
	void OnControl();
	UFUNCTION()
	void OnTalk();
	UFUNCTION()
	void OnAttack();
	UFUNCTION()
	void OnPickPocket();
	UFUNCTION()
	void OnSneak();
	UFUNCTION()
	void OnAutoToggle();
};

