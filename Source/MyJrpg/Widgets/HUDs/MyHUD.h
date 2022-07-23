// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "BaseHUD.h"
#include "GameFramework/HUD.h"
#include "MyJrpg/Widgets/World/WidgetCanvasWorld.h"

#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AMyHUD : public ABaseHUD
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

	virtual void PrintInfoText(const FString& str) override;

	virtual void OpenItemInfo(const FItemDataRow& item_data) override;
};
