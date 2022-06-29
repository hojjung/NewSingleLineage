#include "PlayerCorpse.h"

APlayerCorpse::APlayerCorpse()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APlayerCorpse::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCorpse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}