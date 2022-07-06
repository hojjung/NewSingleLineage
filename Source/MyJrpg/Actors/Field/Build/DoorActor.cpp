#include "DoorActor.h"

#include "MyJrpg/MyLib.h"

ADoorActor::ADoorActor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh2(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Door_Wood_01.SM_Door_Wood_01'"));

	m_MeshDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshDoor"));
	m_MeshDoor->SetupAttachment(m_MeshComp);
	m_MeshDoor->SetStaticMesh(FoundMesh2.Object);
	m_MeshDoor->SetRelativeLocation(FVector(0,61,148));
	m_MeshDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_DoorTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("m_DoorTrigger"));
	m_DoorTrigger->SetupAttachment(m_MeshComp);
	m_DoorTrigger->SetCollisionProfileName(TEXT("Trigger"));
	m_DoorTrigger->SetSphereRadius(250);
	m_DoorTrigger->SetCanEverAffectNavigation(false);

	PrimaryActorTick.bCanEverTick = true;

	m_bIsDoorOpen = false;

	m_fRot = 0;
}

void ADoorActor::SetBuildData(const FBuildDataRow& data)
{
	Super::SetBuildData(data);

	m_DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnTriggerStart);
	
	m_DoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::OnTriggerEnd);
}

void ADoorActor::OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;

	m_bIsDoorOpen = true;
}

void ADoorActor::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;

	m_bIsDoorOpen = false;
}

void ADoorActor::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if(m_bIsDoorOpen)
	{
		m_fRot -= 400.f * DeltaTime;
		m_fRot = FMath::Max(m_fRot, -125.f);
	}
	else
	{
		m_fRot += 400.f * DeltaTime;
		m_fRot = FMath::Min(m_fRot, 0.f);
	}
	m_MeshDoor->SetRelativeRotation(FRotator(0,m_fRot,0));
}
