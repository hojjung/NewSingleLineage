// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWood02.h"

ADoorWood02::ADoorWood02()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_DoorFrame_Wood_02.SM_DoorFrame_Wood_02'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
