#include "ParticleEffectManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

UParticleSystemComponent* UParticleEffectManager::SpawnEffect(const AActor* wantTarget, bool bAttach,
	UParticleSystem* effect, float scale, FVector offset, FName socket)
{
	return SpawnEffect(wantTarget->GetRootComponent(),bAttach,effect,scale,offset,socket);
}
UParticleSystemComponent* UParticleEffectManager::SpawnEffect(USceneComponent* wantTarget, bool bAttach,
	UParticleSystem* effect, float scale, FVector offset,FName socket)
{
	if(bAttach)
	{
		return UGameplayStatics::SpawnEmitterAttached(effect, wantTarget,socket,offset,FRotator::ZeroRotator,FVector(scale),
			EAttachLocation::KeepRelativeOffset,true,EPSCPoolMethod::AutoRelease,true);
	}
	return SpawnEffect(wantTarget->GetComponentLocation(), effect, scale, offset);
}

UParticleSystemComponent* UParticleEffectManager::SpawnEffect(FVector wantTarget, UParticleSystem* effect, float scale,
	FVector offset)
{
	FVector Loc = wantTarget + offset;

	return UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, Loc,
		FRotator::ZeroRotator,FVector(scale),true,EPSCPoolMethod::AutoRelease);
}

void UParticleEffectManager::SpawnSound(const AActor* wantTarget, USoundBase* sound, float volume)
{
	SpawnSound(wantTarget->GetRootComponent(),sound,volume);
}

void UParticleEffectManager::SpawnSound(const USceneComponent* wantTarget, USoundBase* sound, float volume)
{
	SpawnSound(wantTarget->GetComponentLocation(),sound,volume);
}

void UParticleEffectManager::SpawnSound(FVector wantTarget, USoundBase* sound, float volume)
{
	if (!sound)
	{
		return;
	}
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),sound,wantTarget,FRotator::ZeroRotator,volume);
}