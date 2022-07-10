#include "GridActor.h"

#include "NavAreas/NavArea_Null.h"

AGridActor::AGridActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/GridPlane.GridPlane'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetStaticMeshComponent()->SetCanEverAffectNavigation(false);
	SetMobility(EComponentMobility::Movable);
}

void AGridActor::Show()
{
	GetStaticMeshComponent()->SetVisibility(true);
}

void AGridActor::Hide()
{
	GetStaticMeshComponent()->SetVisibility(false);
}

