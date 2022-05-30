// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "WidgetBuffElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "WidgetBuffPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetBuffPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetBuffElement> m_ClassBuffEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_WrapBox;	

	TMap<UBuff_Base*,UWidgetBuffElement*> m_MapBuff;
protected:
	virtual void NativeOnInitialized() override;
	
	void OnBuffAdd(UBuff_Base* buff);

	void OnBuffRemove(UBuff_Base* buff);
};
