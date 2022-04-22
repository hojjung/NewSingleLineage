// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "Components/CameraDissolve.h"
#include "Components/MyWidgetInteractionComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Logics/BulletPool.h"
#include "MyJrpg/Actors/Field/Bullet.h"
#include "MyJrpg/Actors/Field/InteractActorBase.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidgetComp.h"
#include "MyPlayerPawn.generated.h"


class UPlSkillAuto;
class UPlAttchActorManage;
class UPlayerAnimInst;
UCLASS()
class MYJRPG_API AMyPlayerPawn : public ACombatUnitPawn
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerFocusTarget, IFocusable*);

	FOnPlayerFocusTarget m_OnFocus;
public:
	AMyPlayerPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* m_Light;
	UPROPERTY(VisibleAnywhere)
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_TopCamera;
	UPROPERTY()
	FVector m_Input;
	UPROPERTY()
	bool m_bOnceMoveLock;
	UPROPERTY()
	bool m_bIsManulMoving;
	UPROPERTY()
	bool m_bIsSkillUsing;
	UPROPERTY()
	bool m_bCanMoveInSkill;
	UPROPERTY()
	bool m_bIsInvincible;
	UPROPERTY()
	float m_fAddAtkRange;
	UPROPERTY()
	float m_fRangeAttackRange;
	UPROPERTY()
	FVector m_DeltaX;
	UPROPERTY()
	FVector m_DeltaY;
	UPROPERTY()
	UPlSkillAuto* m_SkillAuto;
	UPROPERTY()
	UPlayerAnimInst* m_AnimInst;
	UPROPERTY()
	UPlAttchActorManage* m_AttchActorMng;
	
	FTimerHandle m_InvincTimer;

	TWeakObjectPtr<ACombatUnitPawn> m_LastAttacker;

	bool m_bIsSneaking;

private:
	void DealBaseMeleeAttack();

	void ShootBaseRangeAttack();

	virtual UAnimMontage* GetBaseAttackMontage() override;
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);
	
	virtual void ShowPopupText(float nbr, ETextType t) override;

public:
	virtual void SetFocusedTarget(IFocusable* target) override;
	
	virtual void SetPlayerEntity(const FPlayerUnitEntityRow& unitEntityRow);

	void SetPet(const FPetRow& petRow);

	void UnEquipPet();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//
	bool IsInputMoving();

	void SetPlayerSkMeshDefault();

public://Combat
	virtual float TryAttack() override;

	bool IsManualMoving();

	void SetAutoCombat(bool useAuto);

	void SetPlayerAsset(FName keyId);

	virtual void PlayTookHitMontage() override;

public:
	void SetSkillUsing(bool b);

	void SetCanMoveInSkill(bool b);

	bool GetSkillUsing();

	bool CanMoveInSkill();

	void UpdateStat(const FStatGroup& stat_group);
	
	virtual bool TakeDmg(float amount, ACombatUnitPawn* attacker) override;

	virtual void OnNotifyTrigger(const FName& name) override;
	
	void AddAtkRange(float r);
	
	void SubAtkRange(float r);

	virtual float GetAttackRange() override;

	void TakeHeal(float v);
	
	void TakeInvincible(float d);

	void StopInvincible();
	
	float PlaySkillAnim(const FName& skillID);

	float GetRangeRange();

	virtual void OnDeathAnimEnd() override;

	void CreateRangeBullet(UParticleSystem* effectBullet);

	void RemoveBulletPool();

	virtual bool IsRange() override;

	virtual bool IsSneak() const override;

	void SetSneak();
};


