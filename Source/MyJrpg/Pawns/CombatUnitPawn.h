// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "BaseUnitPawn.h"
#include "Logics/BulletPool.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "CombatUnitPawn.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSkillTrigger,FName);

UCLASS()
class MYJRPG_API ACombatUnitPawn : public ABaseUnitPawn
{
	GENERATED_BODY()
public:
	ACombatUnitPawn(const FObjectInitializer& objInit);

	
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
	float m_fAttackCD;
	UPROPERTY()
	float m_fHitAnimCD;
	UPROPERTY()
	float m_fAttackMinCD;
	UPROPERTY()
	float m_fAttackRange;
	UPROPERTY()
	float m_fDeathAnimDurationMax;
	UPROPERTY()
	float m_fDeathAnimDurationTimer;
	UPROPERTY()
	bool m_bUseFsmTick;
	UPROPERTY()
	bool m_bCanUseSkill;
	UPROPERTY()
	UBulletPool* m_Pool;
	
	FTimerHandle m_DeathAnimTimer;

	FTimerHandle m_StunTimer;

	FFloatCurve m_CurveDeathAnim;

	FTimerHandle m_SilenceTimer;
	
	TWeakObjectPtr<ACombatUnitPawn> m_FocusedTarget;//TScriptInterface<IInteractable>
	
	FSkillTrigger m_OnSkillTrigger;
	
	FStatGroup m_StatGroup;

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
	
	void HomingRotateToTarget();
public://took dmg	
	virtual bool TakeDmg(float amount,ACombatUnitPawn* attacker);
	
	virtual void Dead();

public:// try atk
	virtual float TryAttack();
	//애님 몽타쥬에게 트리거로 타이밍 받음
	virtual void OnNotifyTrigger(const FName& name);
	
public:// get
	virtual bool IsAlive();
	
	virtual float GetAttackRange();
	
	float GetHpPercent() const;

	virtual void SetFocusedTarget(ACombatUnitPawn* target);
    
	virtual ACombatUnitPawn* GetFocusedTarget();

	FVector GetFocusedActorLocation() const;

	FRotator GetFocusedActorRotation() const;

	void SetEnableFsm(bool useFsm);
	
	void PlayDeathAnim();

	void TakeStun(float duration);

	void StopStun();

	void TakeSilence(float duration);

	void StopSilence();

public:
	FSkillTrigger& GetSkillTriggerDelegate();

	const FStatGroup& GetStat() const;

	virtual bool IsRangeMode();

	USceneComponent* GetBulletTarget();

public:
	const TArray<AActor*>& GetTraceIgnoredActors() const;

	const TArray<TEnumAsByte< EObjectTypeQuery>>& GetTraceObjTypes() const;
};
