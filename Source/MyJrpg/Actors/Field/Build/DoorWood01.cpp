// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWood01.h"

ADoorWood01::ADoorWood01()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_DoorFrame_Wood_01.SM_DoorFrame_Wood_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh2(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Door_Wood_01.SM_Door_Wood_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);

	m_MeshDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshDoor"));
	m_MeshDoor->SetupAttachment(GetStaticMeshComponent());
	m_MeshDoor->SetStaticMesh(FoundMesh2.Object);
	m_MeshDoor->SetRelativeLocation(FVector(176,-61,148));
	//(X=176.000000,Y=-61.000000,Z=148.000000)
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
