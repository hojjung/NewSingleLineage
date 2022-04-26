// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWood01.h"

#include "MyJrpg/MyLib.h"

ADoorWood01::ADoorWood01()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_DoorFrame_Wood_01.SM_DoorFrame_Wood_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh2(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Door_Wood_01.SM_Door_Wood_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);

	m_MeshDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshDoor"));
	m_MeshDoor->SetupAttachment(GetStaticMeshComponent());
	m_MeshDoor->SetStaticMesh(FoundMesh2.Object);
	m_MeshDoor->SetRelativeLocation(FVector(0,61,148));
	m_MeshDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_DoorTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("m_DoorTrigger"));
	m_DoorTrigger->SetupAttachment(GetStaticMeshComponent());
	m_DoorTrigger->SetCollisionProfileName(TEXT("Trigger"));
	m_DoorTrigger->SetSphereRadius(250);

	PrimaryActorTick.bCanEverTick = true;

	m_bIsDoorOpen = false;

	m_fRot = 0;
}

void ADoorWood01::BeginPlay()
{
	Super::BeginPlay();

	m_AryDoorMats = m_MeshDoor->GetMaterials();

	m_DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorWood01::OnTriggerStart);
	
	m_DoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorWood01::OnTriggerEnd);
}

void ADoorWood01::SetMat(UMaterialInterface* mat)
{
	Super::SetMat(mat);

	int Iter = 0;

	while (Iter < m_MeshDoor->GetMaterials().Num())
	{
		m_MeshDoor->SetMaterial(Iter,mat);
		Iter++;
	}
}

void ADoorWood01::ConfirmBuild()
{
	Super::ConfirmBuild();

	int Iter = 0;

	while (Iter < m_MeshDoor->GetMaterials().Num())
	{
		m_MeshDoor->SetMaterial(Iter,m_AryDoorMats[Iter]);
		Iter++;
	}
}

void ADoorWood01::OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;

	m_bIsDoorOpen = true;
}

void ADoorWood01::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != UMyLib::GetPlayer())
		return;

	m_bIsDoorOpen = false;
}

void ADoorWood01::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if(m_bIsDoorOpen)
	{
		m_fRot -= 200.f * DeltaTime;
		m_fRot = FMath::Max(m_fRot, -125.f);
	}
	else
	{
		m_fRot += 200.f * DeltaTime;
		m_fRot = FMath::Min(m_fRot, 0.f);
	}
	m_MeshDoor->SetRelativeRotation(FRotator(0,m_fRot,0));
}