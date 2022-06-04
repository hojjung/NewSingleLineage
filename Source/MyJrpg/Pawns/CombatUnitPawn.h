// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "BaseUnitPawn.h"
#include "Logics/BulletPool.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "CombatUnitPawn.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSkillTrigger,FName);

UCLASS()
class MYJRPG_API ACombatUnitPawn : public ABaseUnitPawn,  public IFocusable
{
	GENERATED_BODY()
public:
	ACombatUnitPawn(const FObjectInitializer& objInit);

	DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* m_BulletTarget;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	UPROPERTY()
	TArray<AActor*> m_AryIgnores;
	UPROPERTY()
	UAI_LogicBase* m_AiFsm;
	UPROPERTY()
	USensor_LogicBase* m_AiSensor;
	UPROPERTY()
	UInteractBase* m_Interaction;
	UPROPERTY()
	UBulletPool* m_Pool;
	
	FTimerHandle m_DeathAnimTimer;

	FTimerHandle m_StunTimer;

	FFloatCurve m_CurveDeathAnim;

	FTimerHandle m_SilenceTimer;
	UPROPERTY()
	TScriptInterface<IFocusable> m_FocusedTarget;//TScriptInterface<IInteractable>
	
	FSkillTrigger m_OnSkillTrigger;
	
	FStatGroup m_StatGroup;

	FName m_TeamID;
	
	float m_fAttackCD;
	float m_fHitAnimCD;
	float m_fAttackMinCD;
	float m_fAttackRange;
	float m_fAttackRangeSqr;
	float m_fDeathAnimDurationMax;
	float m_fDeathAnimDurationTimer;
	bool m_bUseFsmTick;
	bool m_bCanUseSkill;

	EStanceType m_Stance;
	
private:
	void CreateSetDeathCurve(float fullLength);
	
	void SetDeathEffectMaterial(float deltaTime);
	
protected:
	bool TryHit(const FStatGroup& other);

	float GetCriticalDmg(float amount);

	float GetReductDmg(float amount);

	float GetRandomDmg(float origin);

	virtual  void PlayTookHitMontage();
	
	virtual void OnDeathAnimEnd();

	void StartDeathEffectMaterial(float duration);

	float PlayBaseAttackAnim();

	virtual UAnimMontage* GetBaseAttackMontage();

	virtual void ShowPopupText(float nbr, ETextType t);

public:
	virtual void SetEntity(const FName& id, const FNpcUnitEntityRow& unitEntityRow) override;

	virtual void Tick(float DeltaSeconds) override;
	
	void HomingRotateToTarget(float speedTime = 5.5f);
	
public://took dmg	
	virtual bool TakeDmg(float amount,ACombatUnitPawn* attacker);
	
	virtual void Dead();

public:// try atk
	virtual float TryAttack();
	//애님 몽타쥬에게 트리거로 타이밍 받음
	virtual void OnNotifyTrigger(const FName& name);
	
public:// get
	virtual bool IsAlive();

	void SetAttackRange(float range);
	
	float GetAttackRange();

	float GetAttackRangeSqr();
	
	float GetHpPercent() const;

	int GetHp();

	virtual void SetFocusedTarget(IFocusable* target);

	FVector GetFocusedActorLocation() const;

	FRotator GetFocusedActorRotation() const;

	void SetEnableFsm(bool useFsm);
	
	void PlayDeathAnim();

	void TakeStun(float duration);

	void StopStun();

	void TakeSilence(float duration);

	void StopSilence();
	
public:
	template<typename T = IFocusable>
	T* GetFocusedTarget() const
	{
		return Cast<T>(m_FocusedTarget.GetObject());
	}
	
public:
	FSkillTrigger& GetSkillTriggerDelegate();

	const FStatGroup& GetStat() const;

	virtual bool IsRangeMode();

	USceneComponent* GetBulletTarget();

public:
	const TArray<AActor*>& GetTraceIgnoredActors() const;

	const TArray<TEnumAsByte< EObjectTypeQuery>>& GetTraceObjTypes() const;

	const FName& GetTeamID() const;

	virtual bool IsSneak() const;

	EPathFollowingRequestResult::Type ChaseTarget();

	virtual EStanceType GetStance();
};
