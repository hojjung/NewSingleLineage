// Fill out your copyright notice in the Description page of Project Settings.


#include "WallWood02.h"

AWallWood02::AWallWood02()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Wall_Wood_02.SM_Wall_Wood_02'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
