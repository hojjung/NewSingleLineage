// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "WidgetCooldownProgress.generated.h"




class UMaterialInstanceDynamic;

UCLASS()
class MYJRPG_API UWidgetCooldownProgress : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWidgetCooldownProgress(const FObjectInitializer& obj);
    
	virtual void NativePreConstruct() override;
    
	virtual void NativeOnInitialized() override;
    
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* m_MatTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImageCooldown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor m_ColorText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor m_ColorProgress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fFontSize;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* m_MatInst;
	UPROPERTY()
	float  m_fCurrentCD;
	UPROPERTY()
	float  m_fMaxCD;

protected:
	UFUNCTION(BlueprintCallable)
	void CreateRenderMat();

	void UpdateProgress();
	
	UFUNCTION(BlueprintCallable)
	void SetFontSize(float size);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UFUNCTION(BlueprintCallable)
	void StartCooldown(float maxCd);
	UFUNCTION(BlueprintCallable)
	void CompleteCooldown();

	bool IsCooldown();
};
