// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowWood01.h"

AWindowWood01::AWindowWood01()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/10_Construction/Meshes/SM_WindowFrame_Wood_01.SM_WindowFrame_Wood_01'"));

	GetStaticMeshComponent()->SetStaticMesh(FoundMesh.Object);
}
