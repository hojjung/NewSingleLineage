// Fill out your copyright notice in the Description page of Project Settings.


#include "WallWood01.h"

AWallWood01::AWallWood01()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_Wall_Wood_01.SM_Wall_Wood_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
