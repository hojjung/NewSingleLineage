// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MyJrpg/MyJrpg.h"

#include "InGameTextWidgetComp.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UInGameTextWidgetComp : public UWidgetComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> m_ClassTextWidget;
	
public:
	UInGameTextWidgetComp();
	
	void SetTextWant(const FText& textWant,ETextType dmg);
	
	virtual void BeginPlay() override;
};
