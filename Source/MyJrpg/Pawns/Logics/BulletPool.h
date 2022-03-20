// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/Bullet.h"
#include "UObject/NoExportTypes.h"
#include "BulletPool.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBulletPool : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ACombatUnitPawn* m_Owner;
	UPROPERTY()
	UParticleSystem* m_Particle;
	UPROPERTY()
	TArray<ABullet*> m_AryBullets;
	UPROPERTY()
	int m_nPoolIndex;
	
public:
	void InitPool(float dmg, int count, UParticleSystem* particle, ACombatUnitPawn* owner, float scale = 1);

	void ShootBullet(FName socketStart,ACombatUnitPawn* target);

	void ShootBullet(FVector start,ACombatUnitPawn* target);

	void ShootBullet(FName socketStart,FVector start,ACombatUnitPawn* target);

	void HideAll();

	void KillAll();
};
