// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "MaterialProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMaterialProgressBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UMaterialProgressBar(const FObjectInitializer& objInit);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D m_MatSize;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName m_NameProgressValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn="true"))
	UMaterialInterface* m_MatWant;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Transient)
	UMaterialInstanceDynamic* m_MatInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImageBar;

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetProgressValue(float v);
	
	virtual bool Initialize() override;

	void SetMatToBrush();
	
	void CreateDynamicMat();
};
