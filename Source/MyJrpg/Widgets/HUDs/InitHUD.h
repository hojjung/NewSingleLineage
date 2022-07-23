// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "BaseHUD.h"
#include "GameFramework/HUD.h"
#include "InitHUD.generated.h"

class UWidgetCanvasInit;
/**
 * 
 */
UCLASS()
class MYJRPG_API AInitHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	AInitHUD();

protected:
	UPROPERTY()
	TSubclassOf<UWidgetCanvasInit> m_ClassWidget;
	
	UPROPERTY()
	UWidgetCanvasInit* m_Canvas;
	
public:
	virtual void BeginPlay() override;

	virtual void PrintInfoText(const FString& str) override;
};
