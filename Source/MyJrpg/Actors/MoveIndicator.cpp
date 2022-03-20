#include "MoveIndicator.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"

AMoveIndicator::AMoveIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	FoundW(TEXT("Material'/Game/03_VisualEffect/Decals/M_SkillIndicator.M_SkillIndicator'"));

	m_Decal = CreateDefaultSubobject<UDecalComponent>("Decal01");
	RootComponent = m_Decal;
	//m_Decal->SetupAttachment(RootComponent);
	m_Decal->SetRelativeRotation(FRotator(-90,0,0));
	m_Decal->SetDecalMaterial(FoundW.Object);

	m_Decal->DecalSize = FVector(5000,51.2f,51.2f);
}
