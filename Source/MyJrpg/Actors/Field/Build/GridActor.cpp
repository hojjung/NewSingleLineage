#include "GridActor.h"

#include "NavAreas/NavArea_Null.h"

AGridActor::AGridActor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/GridPlane.GridPlane'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

