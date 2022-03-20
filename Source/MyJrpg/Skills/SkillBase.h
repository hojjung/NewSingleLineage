#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyJrpg/Managers/PlayerStatusManager.h"
#include "SkillBase.generated.h"

class UPlayerStatusManager;
struct FSkillDataRow;
UCLASS()
class MYJRPG_API USkillBase : public UObject
{
	GENERATED_BODY()
	
protected:
	const FSkillDataRow* m_SkillData;

	int m_nSkillLevel;

	UPROPERTY()
	AMyPlayerPawn* m_Player;
	
private:
	FName m_Id;
	
	FDelegateHandle m_SkillHandle;

	FTimerHandle m_SkillEnd;

	float m_fCooltime;
	
public:
	virtual void SetSkill(FName id,const FSkillDataRow& dataRow, int skillLevel);
	
	virtual void UseSkill();

	virtual bool CanUseSkill();
	
	virtual void Tick(float delta_time);

protected:
	void CheckSkillTriggered(FName id);

	virtual void OnSkillTriggered();

	void PlayAnimMontage();

	bool TraceBox(TArray<AActor*>& outHits,float range, FVector boxHalfSize,TSubclassOf<AMonsterPawn> classFilter = AMonsterPawn::StaticClass());

	bool TraceSphere(TArray<AActor*>& outHits,float range, float radius,TSubclassOf<AMonsterPawn> classFilter = AMonsterPawn::StaticClass());

	bool TraceSphere(TArray<AActor*>& outHits,FVector start, float radius,TSubclassOf<AMonsterPawn> classFilter = AMonsterPawn::StaticClass());

	virtual void EndSkillAnim();

	UPlayerStatusManager* GetPlayerStatus();

	bool IsTargetInRange();

	float GetDamage();
public:
	const FSkillDataRow& GetSkillData() const;

	bool CheckCooltime() const;

	float GetCrnCooltime() const;

	float GetMaxCooltime() const;
};
