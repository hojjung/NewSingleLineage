#include "IndoorBase.h"
#include "MyJrpg/MyLib.h"
#include "NavAreas/NavArea_Null.h"

AIndoorBase::AIndoorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	m_BoxIndoor = CreateDefaultSubobject<UBoxComponent>(TEXT("m_BoxIndoor"));
	RootComponent = m_BoxIndoor; 
	m_BoxIndoor->SetCollisionProfileName(TEXT("Trigger"));
	m_BoxIndoor->SetRelativeLocation(FVector(0,0,25));
	m_BoxIndoor->SetVisibility(false);
	m_BoxIndoor->SetCanEverAffectNavigation(false);
	m_BoxIndoor->AreaClass = UNavArea_Null::StaticClass();
}

void AIndoorBase::BeginPlay()
{
	Super::BeginPlay();

	m_BoxIndoor->OnComponentBeginOverlap.AddDynamic(this, &AIndoorBase::OnTriggerStart);
	m_BoxIndoor->OnComponentEndOverlap.AddDynamic(this, &AIndoorBase::OnTriggerEnd);
}

void AIndoorBase::OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;

	for(UStaticMeshComponent* StMesh : m_AryRoofs)
	{
		StMesh->SetVisibility(false);
	}
}

void AIndoorBase::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;

	for(UStaticMeshComponent* StMesh : m_AryRoofs)
	{
		StMesh->SetVisibility(true);
	}
}
