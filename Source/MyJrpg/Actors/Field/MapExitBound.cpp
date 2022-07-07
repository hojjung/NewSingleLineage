#include "MapExitBound.h"

#include "Components/DecalComponent.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "NavAreas/NavArea_Null.h"

// Sets default values
AMapExitBound::AMapExitBound()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMesh(TEXT("Material'/Game/03_VisualEffect/MAT_Exit.MAT_Exit'"));

	m_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("m_Decal"));
	RootComponent = m_Decal;
	m_Decal->SetRelativeRotation(FRotator(90,180,0));
	m_Decal->SetDecalMaterial(FoundMesh.Object);
	m_Decal->DecalSize = FVector(700,2048,200);
	
	m_CollBox = CreateDefaultSubobject<UBoxComponent>(TEXT("m_CollBox"));
	m_CollBox->SetBoxExtent(m_Decal->DecalSize);
	m_CollBox->SetupAttachment(m_Decal);
	m_CollBox->SetCollisionProfileName(TEXT("Trigger"));
	m_CollBox->SetRelativeLocation(FVector(0,0,25));
	m_CollBox->SetVisibility(false);
	m_CollBox->SetCanEverAffectNavigation(false);
	m_CollBox->AreaClass = UNavArea_Null::StaticClass();
	
	m_TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("m_TextRender"));
	m_TextRender->SetupAttachment(RootComponent);
	m_TextRender->SetWorldSize(250);
	m_TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	m_TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	m_TextRender->SetText(  FText::FromString(TEXT("World Map")));
	m_TextRender->SetRelativeLocation(FVector(1,0,0));
	m_TextRender->CastShadow = false;
}

void AMapExitBound::BeginPlay()
{
	Super::BeginPlay();
	m_CollBox->SetBoxExtent(m_Decal->DecalSize);
	m_CollBox->OnComponentBeginOverlap.AddDynamic(this, &AMapExitBound::OnTriggerStart);
	m_CollBox->OnComponentEndOverlap.AddDynamic(this, &AMapExitBound::OnTriggerEnd);
}

void AMapExitBound::OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != UMyLib::GetPlayer() || UMyLib::GetPlayer()->IsUseFsm())
		return;

	UMyLib::GetCanvas()->GetScreenEffect()->ShowFadeOut(2.5f,FVoidVoid::CreateUObject(this, &AMapExitBound::MoveToMapLevel));
}

void AMapExitBound::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;
	UMyLib::GetCanvas()->GetScreenEffect()->HideFadeOut();
}

void AMapExitBound::MoveToMapLevel()
{
	UMyGameInstance::Get->m_LevelMoveManager->OpenMyLevel(TEXT("MapSelect"));
}
