// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/MyJrpg.h"
#include "InGameTextWidget.generated.h"

UCLASS()
class MYJRPG_API UInGameTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FOnAnimEnd);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* Dmg;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* Critical;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* Heal;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* Miss;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* Immune;
	
	FOnAnimEnd m_OnAnimEnd;

protected:
	typedef void (UInGameTextWidget::*FDmgTxtPtr)(void);

	FDmgTxtPtr m_AryDmgTxtFuncs[static_cast<int>(ETextType::Length)];

	virtual void NativeOnInitialized() override;

public:
	void PlayNormalDmg();

	void PlayCriticalDmg();

	void PlayPlayerHeal();

	void PlayImmune();

	void PlayMiss();

public:
	void SetTextWant(const FText& textWant, ETextType dmg);
	
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
};
