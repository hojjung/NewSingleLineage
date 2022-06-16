// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "CombatUnitPawn.h"
#include "Components/MyFlockSteering.h"
#include "MonsterPawn.generated.h"

/**
 * GetWorldTimerManager().SetTimer(m_DeathAnimTimer, this, &ACombatUnitPawn::OnDeathAnimEnd, AnimLength, false);
 */
UCLASS()
class MYJRPG_API AMonsterPawn : public ACombatUnitPawn
{
	GENERATED_BODY()
	
public:
	AMonsterPawn(const FObjectInitializer& obj);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshRightHand;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshLeftHand;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshBackHand;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UWidgetPawnInfoComp* m_PawnInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* m_SoundComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_HitParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetSpeechBubbleComp* m_SpeechBubbleComp;
	UPROPERTY()
	float m_fMaxHp;
	UPROPERTY()
	float m_fExp;
	UPROPERTY()
	FVector m_SpawnPoint;
	UPROPERTY()
	FRotator m_SpawnRot;
	UPROPERTY()
	bool m_bIsBoss;
	UPROPERTY()
	UInventory* m_Inven;

	FName m_TalkID;
	
protected:
	virtual void BeginPlay() override;
	
	void AttachWeapons();
	
	void PlayHitFlash();
	
	void PlayHittenSound();

	void PlayHittenEffect();

	void CreateInventory();

	virtual void OnDeathAnimEnd() override;

	void OnRequestMoveDone();
	
public:
	virtual void SetFocusedTarget(IFocusable* target) override;
	
	void Speech(FText text);

	void Speech(FString text);

	virtual void SetEntity(const FName& id,const FNpcUnitEntityRow& unitEntityRow) override;

	virtual void Dead() override;
	
	virtual bool TakeDmg(float amount, ACombatUnitPawn* attacker) override;

	virtual void OnNotifyTrigger(const FName& name) override;

	float GetRewardExp() const;

	bool IsBoss() const;
	
	const FName& GetTalkID() const;

	UInventory* GetInven();
	
	void SetHp(int hp);
	
	void SetInven(UInventory* inven);

	void SetDeadBody();

	virtual void OnInteract() override;

	virtual FText GetTextInteract() override;
};


