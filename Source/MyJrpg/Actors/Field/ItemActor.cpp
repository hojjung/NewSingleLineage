#include "ItemActor.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::Obtain()
{
	
}

bool AItemActor::HasOwnerTeamID() const
{
	return !m_OwnerID.IsNone();
}
