// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureBase.h"

AFurnitureBase::AFurnitureBase()
{
	m_BuildData = nullptr;

	m_WidgetComp = CreateDefaultSubobject<UBuildWidgetCompo>(TEXT("m_WidgetComp"));
	m_WidgetComp->SetupAttachment(RootComponent);
	m_WidgetComp->SetRelativeLocation(FVector(0,0,300));
}

void AFurnitureBase::BeginPlay()
{
	Super::BeginPlay();

	GetComponents(m_AryMeshCompos);

	m_AryAryMats.Reset(5);
	
	for(UMeshComponent* MeshComp : m_AryMeshCompos)
	{
		m_AryAryMats.Emplace(MeshComp->GetMaterials());
	}
}

const FBuildDataRow& AFurnitureBase::GetBuildData() const
{
	return *m_BuildData;
}

void AFurnitureBase::SetBuildData(const FBuildDataRow& data)
{
	m_BuildData = &data;
	m_WidgetComp->Init();
}

void AFurnitureBase::SetMat(UMaterialInterface* mat)
{
	int Iter = 0;

	int IterY = 0;

	if (mat)
	{
		while (Iter < m_AryMeshCompos.Num())
		{
			while (IterY < m_AryMeshCompos[Iter]->GetMaterials().Num())
			{
				m_AryMeshCompos[Iter]->SetMaterial(IterY, mat);
				IterY++;
			}
			IterY = 0;
			Iter++;
		}
	}
	else
	{
		while (Iter < m_AryMeshCompos.Num())
		{
			while (IterY < m_AryMeshCompos[Iter]->GetMaterials().Num())
			{
				m_AryMeshCompos[Iter]->SetMaterial(IterY, m_AryAryMats[Iter][IterY].Get());
				IterY++;
			}
			IterY = 0;
			Iter++;
		}
	}
}

void AFurnitureBase::ShowBuildWidget(bool b)
{
	m_WidgetComp->ShowBuildWidget(b);
}

void AFurnitureBase::ConfirmBuild()
{
	m_WidgetComp->SetVisibility(false);
}

void AFurnitureBase::ShowSelect(bool b)
{
	m_WidgetComp->ShowSelect(b);
}

void AFurnitureBase::SetColl(bool b)
{
	if(b)
	{
		m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AFurnitureBase::IsEraseable()
{
	return IBuildable::IsEraseable();
}
