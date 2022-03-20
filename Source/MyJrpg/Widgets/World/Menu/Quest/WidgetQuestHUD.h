#pragma once

#include "CoreMinimal.h"
#include "WidgetQuestHUDChild.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "WidgetQuestHUD.generated.h"

class UQuestLogicBase;
/**
 * 좌상단에 계속 떠있는 역할 
 */
UCLASS()
class MYJRPG_API UWidgetQuestHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetQuestHUDChild> m_ClassHudChild; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_VertQuests;
protected:
	virtual void NativeOnInitialized() override;

	void CreateHUD();
public:
	void DeleteChild(UQuestLogicBase* q,bool isMain);

	void AddChild(UQuestLogicBase* q,bool isMain);
};
