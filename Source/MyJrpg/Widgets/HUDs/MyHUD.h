// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyJrpg/Widgets/World/WidgetCanvasWorld.h"

#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();

protected:
	UPROPERTY()
	TSubclassOf<UWidgetCanvasWorld> m_ClassWidget;
	UPROPERTY()
	UWidgetCanvasWorld* m_Canvas;
	
public:
	virtual void BeginPlay() override;

	UWidgetCanvasWorld* GetCanvas();
	
};
