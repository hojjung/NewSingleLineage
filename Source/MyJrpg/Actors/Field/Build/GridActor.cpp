#include "GridActor.h"

AGridActor::AGridActor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/GridPlane.GridPlane'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
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

