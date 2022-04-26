// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildWidgetCompo.h"

#include "MyJrpg/Actors/Field/Build/StructureActor.h"

UBuildWidgetCompo::UBuildWidgetCompo()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
	TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Build/WBW_Build.WBW_Build_C'"));
    
	m_ClassWidget = FoundW.Class;

	SetDrawAtDesiredSize(true);
	SetPivot(FVector2D (0.5f, 0.5f));
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCanEverAffectNavigation(false);
	SetManuallyRedraw(true);
}

void UBuildWidgetCompo::BeginPlay()
{
	Super::BeginPlay();
	SetWidgetClass(m_ClassWidget);
	m_Widget = Cast<UWorldWidgetStruct>(GetUserWidgetObject());
	m_Widget->SetOwnerActor(GetOwner<AStructureActor>());
}

void UBuildWidgetCompo::ShowRotation(bool b)
{
	m_Widget->ShowRotation(b);
}

void UBuildWidgetCompo::ShowBuildWidget(bool b)
{
	m_Widget->ShowBuildWidget(b);
}

void UBuildWidgetCompo::ShowSelect(bool b)
{
	m_Widget->ShowSelect(b);
}
