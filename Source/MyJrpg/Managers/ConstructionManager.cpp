#include "ConstructionManager.h"

#include "Editor/GroupActor.h"
#include "Engine/StaticMeshActor.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/Build/GridActor.h"
#include "MyJrpg/DataTables/BuildData.h"

FConEle::FConEle(): m_Foundation(nullptr), m_Prop(nullptr)
{
	m_Walls.Init(nullptr, (int)UConstructionManager::EWallDir::Length);
}

UConstructionManager::UConstructionManager()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMat01(TEXT("Material'/Game/10_Construction/Materials/M_Enabled.M_Enabled'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMat02(TEXT("Material'/Game/10_Construction/Materials/M_Disabled.M_Disabled'"));
	
	m_MatGreen = FoundMat01.Object;
	m_MatRed = FoundMat02.Object;
}

void UConstructionManager::Init()
{
	UBuildData::GetBuildTable->GetAllRows<FBuildDataRow>("", m_AryBuildDatas);
}

void UConstructionManager::LoadConstruction()
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_GridMesh = GetWorld()->SpawnActor<AGridActor>(AGridActor::StaticClass(), FVector(0,0,20), FRotator::ZeroRotator, Param);
	m_GridMesh->SetActorScale3D(FVector(50));
	EndBuilding();
}

void UConstructionManager::StartBuilding()
{
	m_GridMesh->Show();
}

void UConstructionManager::EndBuilding()
{
	m_GridMesh->Hide();
	
	if(m_PreviewActor)
	{
		m_PreviewActor->Destroy();
		m_PreviewActor = nullptr;
	}
}

FVector UConstructionManager::GetWorldPos(int x, int y)
{
	return FVector((y * FGlobalVariable::GRID_SIZE) - 2500 + (FGlobalVariable::GRID_SIZE / 2.f), (x * FGlobalVariable::GRID_SIZE) - 2500 + (FGlobalVariable::GRID_SIZE / 2.f), 0);
}

void UConstructionManager::GetIndex(const FVector& inloc, int& outX, int& outY)
{
	int Y = (inloc.X + 2500.f) / FGlobalVariable::GRID_SIZE;

	int X = (inloc.Y + 2500.f) / FGlobalVariable::GRID_SIZE;

	outX = FMath::Clamp(X, 0,FGlobalVariable::GRID_COUNT);

	outY = FMath::Clamp(Y, 0,FGlobalVariable::GRID_COUNT);
}

bool UConstructionManager::FindEmptyWallPlace(const FConEle& Ele, EWallDir& dir)
{
	int Iter = 0;
	bool FoundEmpty = false;
	for(AStructureActor* Wall : Ele.m_Walls)
	{
		if(!Wall)
		{
			Iter++;
			continue;
		}
		FoundEmpty = true;
		break;
	}
	dir = (EWallDir)Iter;
	return FoundEmpty;
}

bool UConstructionManager::GetEmptyFoundationLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot)
{
	while (y < FGlobalVariable::GRID_COUNT)
	{
		while (x < FGlobalVariable::GRID_COUNT)
		{
			const FConEle& Ele = m_Grid[x][y];
			if(Ele.m_Foundation)
			{
				x++;
				continue;
			}
			outEmptyLoc = GetWorldPos(x, y);
			outEmptyRot = FRotator::ZeroRotator;
			return true;
		}
		y++;
	}
	return false;
}

bool UConstructionManager::GetEmptyWallLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot)
{
	int IterWall = 0;
	while (y < FGlobalVariable::GRID_COUNT)
	{
		while (x < FGlobalVariable::GRID_COUNT)
		{
			const FConEle& Ele = m_Grid[x][y];
			IterWall = 0;
			for(AStructureActor* Wall : Ele.m_Walls)
			{
				if(Wall)
				{
					IterWall++;
					continue;
				}
				outEmptyLoc = GetWorldPos(x, y);
				outEmptyRot = FRotator(0,IterWall * 90.f,0);
				return true;
			}
			x++;
		}
		y++;
	}
	return false;
}

bool UConstructionManager::GetEmptyLoc(const FVector& inloc, FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t)
{
	int X,Y;
	
	GetIndex(inloc ,X,Y);

	PRINTF("CoordIndex: X:%d, Y:%d", X, Y);

	switch (t)
	{
	case EBuildType::Foundation:
		return GetEmptyFoundationLoc(X,Y,outEmptyLoc, outEmptyRot);
	case EBuildType::Wall:
	case EBuildType::Door:
		return GetEmptyWallLoc(X,Y,outEmptyLoc, outEmptyRot);
	}
	return false;
}

void UConstructionManager::CheckBuildable()
{
	if(IsBuildable())
	{
		m_PreviewActor->SetMat(m_MatGreen);
		return;
	}
	m_PreviewActor->SetMat(m_MatRed);
}

void UConstructionManager::SpawnPreviewActor(FVector loc, const FBuildDataRow& dataRow)
{
	FVector Loc;

	FRotator Rot;

	if(!GetEmptyLoc(loc,Loc, Rot, dataRow.m_BuildType))
	{
		return;
	}

	if (!m_PreviewActor || m_PreviewActor->GetClass() != dataRow.m_ClassActor)
	{
		if(m_PreviewActor)
		{
			m_PreviewActor->Destroy();
			m_PreviewActor = nullptr;
		}
		FActorSpawnParameters Param;
		Param.bNoFail = true;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PreviewActor = GetWorld()->SpawnActor<AStructureActor>(dataRow.m_ClassActor, Param);
		m_PreviewActor->SetActorEnableCollision(false);
		m_PreviewActor->SetActorScale3D(FVector(0.885f));
	}
	m_PreviewActor->SetBuildData(dataRow);
	m_PreviewActor->SetActorLocation(Loc);
	m_PreviewActor->SetActorRotation(Rot);

	CheckBuildable();
}

void UConstructionManager::ConfirmBuild()
{
	m_PreviewActor->ConfirmBuild();
	m_PreviewActor = nullptr;
}

bool UConstructionManager::IsBuildable()
{
	return true;
}
