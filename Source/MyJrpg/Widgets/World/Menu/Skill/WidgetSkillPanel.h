// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "WidgetSkillElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "WidgetSkillPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetSkillPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetSkillElement> m_ClassSkillEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollBox;

	TWeakObjectPtr<UWidgetSkillElement> m_SelectedSkill;
	
protected:
	virtual void NativeOnInitialized() override;

	void OnFocus(UWidgetSkillElement* ele);

	virtual void ClosePanel() override;
	
public:
	void UnFocusCurrent();
	
	void UpdateSkills();

	const FName* GetFocusedSkill()
	{
		return m_SelectedSkill.Get() ? &m_SelectedSkill->GetSkillID() : nullptr;
	}
};
