// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorStone.h"

AFloorStone::AFloorStone()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Floor_Stone_01.SM_Floor_Stone_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
