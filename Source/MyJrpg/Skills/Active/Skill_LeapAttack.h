#pragma once

#include "CoreMinimal.h"
#include "TweenPath.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_LeapAttack.generated.h"

/**
 * 점프 어택
 */
UCLASS()
class MYJRPG_API USkill_LeapAttack : public USkillBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UTweenPath* m_MoveTween;

	FVector m_EndPoint;
	
protected:
	virtual bool CanUseSkill() override;

	virtual void UseSkill() override;

	virtual void EndSkillAnim() override;
	
public:
	virtual void OnSkillTriggered() override;
	UFUNCTION()
	void OnUpdateMove(FVector curPos, UTweenBase* tween);
	UFUNCTION()
	void OnComplete(FVector curPos, UTweenBase* tween);
};