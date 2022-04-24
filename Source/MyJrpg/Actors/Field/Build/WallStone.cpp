// Fill out your copyright notice in the Description page of Project Settings.


#include "WallStone.h"

AWallStone::AWallStone()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Wall_Stone_01.SM_Wall_Stone_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
