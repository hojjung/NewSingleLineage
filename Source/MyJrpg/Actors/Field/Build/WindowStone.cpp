// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowStone.h"

AWindowStone::AWindowStone()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_WindowFrame_Stone_01.SM_WindowFrame_Stone_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
