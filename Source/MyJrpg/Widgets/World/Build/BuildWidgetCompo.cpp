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

void UBuildWidgetCompo::Init()
{
	SetWidgetClass(m_ClassWidget);
	AStructureActor* Owner = GetOwner<AStructureActor>();
	m_Widget = Cast<UWorldWidgetStruct>(GetUserWidgetObject());
	m_Widget->SetOwnerActor(Owner);
	m_Type = Owner->GetBuildData().m_BuildType;
}

void UBuildWidgetCompo::ShowBuildWidget(bool b)
{
	m_Widget->ShowBuildWidget(b);
	m_Widget->ShowRotation(b && m_Type == EBuildType::Furniture);
}

void UBuildWidgetCompo::ShowSelect(bool b)
{
	SetVisibility(b);
	m_Widget->ShowSelect(b);
	m_Widget->ShowRotation(b && m_Type == EBuildType::Furniture);
}
