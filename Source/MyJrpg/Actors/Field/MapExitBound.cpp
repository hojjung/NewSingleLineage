#include "MapExitBound.h"

#include "MyJrpg/MyLib.h"
#include "NavAreas/NavArea_Null.h"

// Sets default values
AMapExitBound::AMapExitBound()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/Exit.Exit'"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
	GetStaticMeshComponent()->SetupAttachment(RootComponent);
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(10));
	GetStaticMeshComponent()->SetRelativeLocation(FVector(0,0,1));
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetStaticMeshComponent()->CastShadow = false;
	m_CollBox = CreateDefaultSubobject<UBoxComponent>(TEXT("m_CollBox"));
	m_CollBox->SetBoxExtent(FVector(50,50,25));
	m_CollBox->SetupAttachment(GetStaticMeshComponent());
	m_CollBox->SetCollisionProfileName(TEXT("Trigger"));
	m_CollBox->SetRelativeLocation(FVector(0,0,25));
	m_CollBox->SetVisibility(false);
	m_CollBox->SetCanEverAffectNavigation(false);
	m_CollBox->AreaClass = UNavArea_Null::StaticClass();
	m_TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("m_TextRender"));
	m_TextRender->SetupAttachment(RootComponent);
	m_TextRender->SetWorldSize(350);
	m_TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	m_TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	m_TextRender->SetText(  FText::FromString(TEXT("World Map")));
	m_TextRender->SetRelativeLocation(FVector(0,0,2));
	m_TextRender->SetRelativeRotation(FRotator(90,0,0));
	m_TextRender->CastShadow = false;
}

// Called when the game starts or when spawned
void AMapExitBound::BeginPlay()
{
	Super::BeginPlay();
	m_CollBox->OnComponentBeginOverlap.AddDynamic(this, &AMapExitBound::OnTriggerStart);
}

void AMapExitBound::OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;
	PRINTF("AMapExitBound::Start");
}
