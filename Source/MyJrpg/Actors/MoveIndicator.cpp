#include "MoveIndicator.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"

AMoveIndicator::AMoveIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("StShadow"));
	m_Decal->SetRelativeRotation(FRotator(90,0,0));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundSt(
			TEXT("Material'/Game/03_VisualEffect/Decals/M_SkillIndicator.M_SkillIndicator'"));
	m_Decal->SetDecalMaterial(FoundSt.Object);
	m_Decal->SetupAttachment(RootComponent);
	m_Decal->SetCanEverAffectNavigation(false);
	m_Decal->DecalSize = FVector(512,128,128);
}
