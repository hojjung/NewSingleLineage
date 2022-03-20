// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ParticleEffectManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UParticleEffectManager : public UObject
{
	GENERATED_BODY()

public:
	UParticleSystemComponent* SpawnEffect(const AActor* wantTarget,bool bAttach,UParticleSystem* effect, float scale = 1 ,FVector offset = FVector::ZeroVector,FName socket = NAME_None);
	
	UParticleSystemComponent* SpawnEffect(USceneComponent* wantTarget,bool bAttach,UParticleSystem* effect, float scale = 1 ,FVector offset = FVector::ZeroVector,FName socket = NAME_None);

	UParticleSystemComponent* SpawnEffect(FVector wantTarget,UParticleSystem* effect, float scale = 1 ,FVector offset = FVector::ZeroVector);

	void SpawnSound(const AActor* wantTarget, USoundBase* sound, float volume = 1);

	void SpawnSound(const USceneComponent* wantTarget, USoundBase* sound, float volume = 1);

	void SpawnSound(FVector wantTarget, USoundBase* sound, float volume = 1);
};
