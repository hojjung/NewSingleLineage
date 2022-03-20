// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetCooldownProgress.h"

#include "WidgetBuffElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetBuffElement : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCooldownProgress* m_BuffDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
public:
	void SetBuff(const USkill_BuffBase* buff);
};
