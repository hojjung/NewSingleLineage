// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "ModularUnitPawn.h"

#include "Components/CameraDissolve.h"
#include "Components/MyWidgetInteractionComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Logics/BulletPool.h"
#include "MyJrpg/Actors/Field/Bullet.h"
#include "MyJrpg/Actors/Field/Build/BuildInteract/BI_StorageSearch.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidgetComp.h"
#include "MyPlayerPawn.generated.h"


class AMoveIndicator;
class UPlSkillAuto;
class UPlAttchActorManage;
class UPlayerAnimInst;


UCLASS()
class MYJRPG_API AMyPlayerPawn : public AModularUnitPawn
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerFocusTarget, IFocusable*);

	FOnPlayerFocusTarget m_OnFocus;
	
public:
	AMyPlayerPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY()
	AMoveIndicator* m_FocusIndicator;
	UPROPERTY(VisibleAnywhere)
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_TopCamera;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_MapCamera;
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
	FVector m_DeltaX;
	UPROPERTY()
	FVector m_DeltaY;
	UPROPERTY()
	UPlSkillAuto* m_SkillAuto;
	
	FTimerHandle m_InvincTimer;

	FTimerHandle m_WaitInteractTimer;

	TWeakObjectPtr<ACombatUnitPawn> m_LastAttacker;

	bool m_bIsSneaking;

	bool m_bIsInteracting;

	FAIRequestID m_ReqID;

	FVoidVoid m_OnRequestDone;

	FVoidVoid m_OnCancelInteract;

	FVector2D m_Offset;

private:
	void CreateFocusActor();
	
	void DealBaseMeleeAttack();

	void ShootBaseRangeAttack();

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);
	
	virtual void ShowPopupText(float nbr, ETextType t) override;
	
	void ShowIndicator(IFocusable* target);

	bool CheckTargetRange();

	void OnRequestMoveDone(FAIRequestID id, const FPathFollowingResult& rslt);

	void TryAttack_External();


public:
	void CancelInteract();
	
	virtual void SetFocusedTarget(IFocusable* target) override;
	
	void SetPlayerEntity();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//
	bool IsInputMoving();

	void SetPlayerSkMeshDefault();

public://Combat
	void RequestAttack();

	bool IsManualMoving();

	void SetAutoCombat(bool useAuto);

	virtual void PlayTookHitMontage() override;

public:
	void SetSkillUsing(bool b);

	void SetCanMoveInSkill(bool b);

	bool GetSkillUsing();

	bool CanMoveInSkill();

	void UpdateStat(const FStatGroup& stat_group);
	
	virtual bool TakeDmg(float amount, ACombatUnitPawn* attacker) override;

	virtual void OnNotifyTrigger(const FName& id) override;
	
	void TakeHeal(float v);
	
	void TakeInvincible(float d);

	void StopInvincible();
	
	float PlaySkillAnim(const FName& skillID);

	virtual void OnDeathAnimEnd() override;

	void CreateRangeBullet(UParticleSystem* effectBullet);

	void RemoveBulletPool();

	virtual bool IsRange() override;

	virtual bool IsSneak() const override;

	void SetSneak();

	void SetInteracting(bool b);

	bool GetInteracting() const;
	
	void WaitInteract(float interactTime, const FVoidVoid& delegate);

	void RequestInteract(AActor* target, const FVoidVoid& delegate, float r = 250.f);

	void BindOnCancel( const FVoidVoid& onCancel);
	
	void UnbindCancel();

public:
	FVector2D GetCameraOffset();
	
	void SetCameraOffset(const FVector2D& vector_2d);

	void ClearCameraOffset();

	virtual bool IsLooting() override;
};

