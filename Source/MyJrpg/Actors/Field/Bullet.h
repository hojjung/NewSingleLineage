// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

class ACombatUnitPawn;
UCLASS()
class MYJRPG_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* m_Effect;
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* m_Move;
	UPROPERTY()
	bool m_bDestoryOnEnd;
	UPROPERTY()
	FVector m_Dest;
	UPROPERTY()
	float m_fDmg;
	
	TWeakObjectPtr<ACombatUnitPawn> m_OwnerUnit;

	TWeakObjectPtr<ACombatUnitPawn> m_TargetUnit;

protected:
	virtual void Tick(float DeltaSeconds) override;

	void OnChaseDone();

	bool IsEqual();
	
public:
	void Init(float dmg, ACombatUnitPawn* ownerUnit, UParticleSystem* effect, bool destoryOnEnd,FVector scale = FVector::OneVector);
	
	void StartBullet(FVector start, ACombatUnitPawn* targetUnit);

	void EndBullet();

	void SetActive(bool isActive);
};
