#include "ConstructionManager.h"
#include "Engine/StaticMeshActor.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/StructureActor.h"
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
				outEmptyRot = FRotator(0, FMath::RoundToFloat(IterWall * 90),0);
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
		m_PreviewActor->ShowBuildWidget(true);
		return;
	}
	m_PreviewActor->SetMat(m_MatRed);
	m_PreviewActor->ShowBuildWidget(false);
}

void UConstructionManager::SpawnPreviewActor(FVector loc, const FBuildDataRow* dataRow)
{
	if(!dataRow)
	{
		if(m_PreviewActor)
			dataRow = &m_PreviewActor->GetBuildData();
		else
			return;
	}
	FVector Loc;

	FRotator Rot;

	GetEmptyLoc(loc,Loc, Rot, dataRow->m_BuildType);

	if (!m_PreviewActor || m_PreviewActor->GetClass() != dataRow->m_ClassActor)
	{
		if(m_PreviewActor)
		{
			m_PreviewActor->Destroy();
			m_PreviewActor = nullptr;
		}
		FActorSpawnParameters Param;
		Param.bNoFail = true;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PreviewActor = GetWorld()->SpawnActor<AStructureActor>(dataRow->m_ClassActor, Param);
		m_PreviewActor->SetActorEnableCollision(false);
		m_PreviewActor->SetActorScale3D(FVector(0.885f));
	}
	m_PreviewActor->SetBuildData(*dataRow);
	m_PreviewActor->SetActorLocation(Loc);
	m_PreviewActor->SetActorRotation(Rot);

	CheckBuildable();
}

bool UConstructionManager::IsBuildable()
{
	if(!m_PreviewActor)
		return false;
	int X,Y;
	
	GetIndex(m_PreviewActor->GetActorLocation(),X,Y);

	FConEle& Ele = m_Grid[X][Y];

	switch (m_PreviewActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
		if(Ele.m_Foundation)
			return false;
			break;
	case EBuildType::Wall:
	case EBuildType::Door:
		int Dir = GetPreviewRotDir();
		if(Ele.m_Walls[Dir])
			return false;
	}
	return true;
}

int UConstructionManager::GetPreviewRotDir()
{
	float Yaw = FMath::RoundToFloat(m_PreviewActor->GetActorRotation().GetDenormalized().Yaw);
	
	int Dir = Yaw / (90);

	if(Dir >= (int)EWallDir::Length)
		Dir = 0;
	else if(Dir < 0)
		Dir = (int)EWallDir::Length - 1;

	return Dir;
}

void UConstructionManager::ConfirmBuild()
{
	int X,Y;

	FVector Loc = m_PreviewActor->GetActorLocation(); 

	GetIndex(Loc,X,Y);

	FConEle& Ele = m_Grid[X][Y];

	switch (m_PreviewActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
		Ele.m_Foundation = m_PreviewActor; 
		break;
	case EBuildType::Wall:
	case EBuildType::Door:
		int Dir = GetPreviewRotDir();
		Ele.m_Walls[Dir] = m_PreviewActor;
		break;
	}
	
	const FBuildDataRow& BuildRow = m_PreviewActor->GetBuildData();
	
	m_PreviewActor->ConfirmBuild();
	m_PreviewActor = nullptr;

	SpawnPreviewActor(Loc, &BuildRow);

	m_OnConfirm.Broadcast();
}

void UConstructionManager::Rotate()
{
	float Yaw = FMath::RoundToFloat(m_PreviewActor->GetActorRotation().GetDenormalized().Yaw);

	int Dir = Yaw / (90);

	Dir++;
	if(Dir >= (int)EWallDir::Length)
		Dir = 0;
	else if(Dir < 0)
		Dir = (int)EWallDir::Length - 1;
		

	m_PreviewActor->SetActorRotation(FRotator(0,90 * Dir,0));
	CheckBuildable();
}

AStructureActor* UConstructionManager::GetPreview()
{
	return m_PreviewActor;
}

void UConstructionManager::Cancel()
{
	if(m_PreviewActor)
	{
		m_PreviewActor->Destroy();
		m_PreviewActor = nullptr;
		m_OnCancel.Broadcast();
	}
}
