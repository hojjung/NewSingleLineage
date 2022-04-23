#include "ConstructionManager.h"

#include "Engine/StaticMeshActor.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/BuildData.h"

FConEle::FConEle(): m_Foundation(nullptr), m_Prop(nullptr)
{
	m_Walls.Init(nullptr, (int)UConstructionManager::EWallDir::Length);
}

UConstructionManager::UConstructionManager()
{
	//Material'/Game/10_Construction/Materials/M_Enabled.M_Enabled'
	//Material'/Game/10_Construction/Materials/M_Disabled.M_Disabled'
	//StaticMesh'/Game/10_Construction/Meshes/GridPlane.GridPlane'
	
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
	m_GridMesh = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Param);
	m_GridMesh->GetStaticMeshComponent()->SetMaterial();
}

void UConstructionManager::RemoveConstruction()
{
	
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
			outEmptyLoc = FVector(x,y,100);
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
				outEmptyLoc = FVector(x,y,100);
				outEmptyRot = FRotator(0,IterWall * 90.f,0);
				return true;
			}
			x++;
		}
		y++;
	}
	return false;
}

bool UConstructionManager::GetEmptyLoc(FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t)
{
	FVector Loc = UMyLib::GetPlayer()->GetActorLocation();

	int X = Loc.X /= FGlobalVariable::GRID_SIZE;

	int Y = Loc.Y /= FGlobalVariable::GRID_SIZE;

	X = FMath::Clamp(X, 0,FGlobalVariable::GRID_COUNT);

	Y = FMath::Clamp(Y, 0,FGlobalVariable::GRID_COUNT);

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

void UConstructionManager::SpawnPreviewActor(const FBuildDataRow& dataRow)
{
	FVector Loc;

	FRotator Rot;
	
	if(!GetEmptyLoc(Loc, Rot, dataRow.m_BuildType))
	{
		return;
	}

	if (!m_PreviewActor)
	{
		FActorSpawnParameters Param;
		Param.bNoFail = true;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PreviewActor = GetWorld()->SpawnActor<AStructureActor>(dataRow.m_ClassActor, Loc, Rot, Param);
	}
	else
	{
		m_PreviewActor->SetActorLocation(Loc);
		m_PreviewActor->SetActorRotation(Rot);
	}
	m_PreviewActor->SetBuildData(dataRow);

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
