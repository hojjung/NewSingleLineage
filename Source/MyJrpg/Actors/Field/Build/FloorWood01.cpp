// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorWood01.h"

AFloorWood01::AFloorWood01()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Floor_Wood_01.SM_Floor_Wood_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
