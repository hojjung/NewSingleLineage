// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InitHUD.generated.h"

class UWidgetCanvasInit;
/**
 * 
 */
UCLASS()
class MYJRPG_API AInitHUD : public AHUD
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
};
