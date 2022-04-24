// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorWood02.h"

AFloorWood02::AFloorWood02()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Floor_Wood_02.SM_Floor_Wood_02'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
