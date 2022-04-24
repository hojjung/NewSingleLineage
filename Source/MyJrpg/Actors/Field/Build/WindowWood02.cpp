// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowWood02.h"

AWindowWood02::AWindowWood02()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_WindowFrame_Wood_02.SM_WindowFrame_Wood_02'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
