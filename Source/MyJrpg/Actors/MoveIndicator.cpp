#include "MoveIndicator.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"

AMoveIndicator::AMoveIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_Decal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StShadow"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
			TEXT("StaticMesh'/Game/03_VisualEffect/Decals/SM_Indicator.SM_Indicator'"));
	m_Decal->SetStaticMesh(FoundSt.Object);
	m_Decal->SetupAttachment(RootComponent);
	m_Decal->SetRelativeLocation(FVector(0,0,1));
	m_Decal->SetRelativeScale3D(FVector(9));
	m_Decal->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Decal->SetCanEverAffectNavigation(false);
}
