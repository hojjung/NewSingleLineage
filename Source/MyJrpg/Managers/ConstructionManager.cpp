#include "ConstructionManager.h"

#include "MyGameInstance.h"
#include "Engine/StaticMeshActor.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/Build/GridActor.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/DataTables/BuildData.h"

FConEle::FConEle(): m_Foundation(nullptr), m_Furniture(nullptr)
{
	
}

FWallAry::FWallAry()
{
	
}

FWallAry::FWallAry(int count)
{
	m_Walls.Init(nullptr, count);
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
	
	m_WallHorizontal.Init(FWallAry(FGlobalVariable::GRID_COUNT + 1),FGlobalVariable::GRID_COUNT);//y x
	m_WallVertical.Init(FWallAry(FGlobalVariable::GRID_COUNT),FGlobalVariable::GRID_COUNT + 1);
}

void UConstructionManager::StartBuilding()
{
	m_GridMesh->Show();
}

void UConstructionManager::EndBuilding()
{
	m_GridMesh->Hide();
	
	Cancel();
	CancelSelect();
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

bool UConstructionManager::GetWallIndex(const FVector& inloc, int& outX, int& outY, bool &isHori)
{
	float X = (inloc.Y + 2500.f) / FGlobalVariable::GRID_SIZE;//14.5 최대, 15.5초과
	
	float Y = (inloc.X + 2500.f) / FGlobalVariable::GRID_SIZE;
	
	float PercentX = X -  (int)X;
	
	float PercentY = Y -  (int)Y;

	if(PercentY <= 0.2f)//s
	{
		isHori = true;
		outX = X;
		outY = Y;
		return true;
	}
	else if (PercentY >= 0.8f)//n
	{
		isHori = true;
		outX = X;
		outY = Y + 1;
		return true;
	}
	isHori = false;
	if(PercentX <= 0.5f)//w
	{
		outX = X;
	}
	else
	{
		outX = X + 1;
	}//e
	outY = Y;
	return true;
}

int UConstructionManager::GetWallMaxIndex(bool ishori, bool isXaxis)
{
	if(ishori)
	{
		if(isXaxis)
		{
			return m_WallHorizontal.Num();
		}
		else
		{
			return m_WallHorizontal[0].m_Walls.Num();
		}
	}
	else
	{
		if(isXaxis)
		{
			return m_WallVertical.Num();
		}
		else
		{
			return m_WallVertical[0].m_Walls.Num();
		}
	}
}

bool UConstructionManager::GetEmptyFoundationLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot)
{
	int initX = x;
	int initY = y;
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
	outEmptyLoc = GetWorldPos(initX,initY);
	outEmptyRot = FRotator::ZeroRotator;
	return false;
}

bool UConstructionManager::GetEmptyFurnitureLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot)
{
	int initX = x;
	int initY = y;
	while (y < FGlobalVariable::GRID_COUNT)
	{
		while (x < FGlobalVariable::GRID_COUNT)
		{
			const FConEle& Ele = m_Grid[x][y];
			if(Ele.m_Furniture)
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
	outEmptyLoc = GetWorldPos(initX,initY);
	outEmptyRot = FRotator::ZeroRotator;
	return false;
}

FVector UConstructionManager::GetWallWorldPos(bool isHori, int wall_x, int wall_y)
{
	FVector Loc;
	
	if(isHori)
	{
		Loc.X = (wall_y * FGlobalVariable::GRID_SIZE) - 2500;
		Loc.Y = (wall_x * FGlobalVariable::GRID_SIZE) - 2500 + (FGlobalVariable::GRID_SIZE / 2.f);
	}
	else
	{
		Loc.X = (wall_y * FGlobalVariable::GRID_SIZE) - 2500+ (FGlobalVariable::GRID_SIZE / 2.f);
		Loc.Y = (wall_x * FGlobalVariable::GRID_SIZE) - 2500;
	}
	Loc.Z = 0;
	return Loc;
}

bool UConstructionManager::GetEmptyWallLoc(int x, int y, bool isHori, FVector& outEmptyLoc, FRotator& outEmptyRot)
{
	int initX = x;
	
	int initY = y;
	
	TArray<FWallAry>* AryWall;
	
	if(!isHori)
	{
		outEmptyRot = FRotator(0,90,0);
		AryWall = &m_WallVertical;
		while (x < AryWall->Num())
		{
			while (y < (*AryWall)[x].m_Walls.Num())
			{
				if((*AryWall)[x].m_Walls[y])
				{
					y++;
					continue;
				}
				outEmptyLoc = GetWallWorldPos(isHori, x, y);
				return true;
			}
			//y = 0;
			x++;
		}
	}
	else
	{
		outEmptyRot = FRotator::ZeroRotator;
		AryWall = &m_WallHorizontal;
		while (y < (*AryWall)[x].m_Walls.Num())
		{
			while (x < AryWall->Num())
			{
				if ((*AryWall)[x].m_Walls[y])
				{
					x++;
					continue;
				}
				outEmptyLoc = GetWallWorldPos(isHori, x, y);
				return true;
			}
			x = initX;
			y++;
		}
	}
	
	outEmptyLoc = GetWallWorldPos(isHori, initX, initY);
	return false;
}

bool UConstructionManager::GetEmptyLoc(const FVector& inloc, FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t)
{
	int X,Y;

	switch (t)
	{
	case EBuildType::Foundation:
		{
			GetIndex(inloc ,X,Y);
			PRINTF("Floor Index:%d:%d",X,Y);
			return GetEmptyFoundationLoc(X,Y,outEmptyLoc, outEmptyRot);
		}
	case EBuildType::Furniture:
		{
			GetIndex(inloc ,X,Y);
			PRINTF("Floor Index:%d:%d",X,Y);
			return GetEmptyFurnitureLoc(X,Y,outEmptyLoc, outEmptyRot);
		}
	case EBuildType::Wall:
	case EBuildType::Door:
		{
			bool IsHori = true;
			GetWallIndex(inloc,X,Y,IsHori);
			PRINTF("Wall Index:%d:%d, IsHori:%d",X,Y,IsHori);
			return GetEmptyWallLoc(X,Y,IsHori,outEmptyLoc, outEmptyRot);
		}
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
	FVector NewLoc;

	FRotator NewRot;

	GetEmptyLoc(loc,NewLoc, NewRot, dataRow->m_BuildType);

	AActor* SelectedActor = Cast<AActor>(m_PreviewActor.GetObject());

	if (!m_PreviewActor || SelectedActor->GetClass() != dataRow->m_ClassActor)
	{
		if(m_PreviewActor)
		{
			SelectedActor->Destroy();
			m_PreviewActor = nullptr;
		}
		IBuildable* SpawnedActor = SpawnStructure(*dataRow);
		m_PreviewActor.SetInterface(SpawnedActor);
		m_PreviewActor.SetObject(Cast<UObject>(SpawnedActor));
		SelectedActor = Cast<AActor>(m_PreviewActor.GetObject());
		SelectedActor->SetActorEnableCollision(false);
	}
	
	SelectedActor->SetActorLocation(NewLoc);
	SelectedActor->SetActorRotation(NewRot);

	CheckBuildable();
}

bool UConstructionManager::IsBuildable()
{
	if(!m_PreviewActor)
		return false;
	int X,Y;
	
	switch (m_PreviewActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
		{
			GetIndex( Cast<AActor>(m_PreviewActor.GetObject())->GetActorLocation(), X, Y);
			FConEle& Ele = m_Grid[X][Y];
			if (Ele.m_Foundation)
				return false;
		}
			break;
	case EBuildType::Wall:
	case EBuildType::Door:
		{
			bool IsHori;
			GetWallIndex(Cast<AActor>(m_PreviewActor.GetObject())->GetActorLocation(), X, Y, IsHori);
			if (IsHori)
			{
				if (!m_Grid[X][Y - 1].m_Foundation && !m_Grid[X][Y].m_Foundation)
					return false;
				if (m_WallHorizontal[X].m_Walls[Y])
					return false;
			}
			else
			{
				if (!m_Grid[X - 1][Y].m_Foundation && !m_Grid[X][Y].m_Foundation)
					return false;
				if (m_WallVertical[X].m_Walls[Y])
					return false;
			}
		}
		break;
	case EBuildType::Furniture:
		{
			GetIndex( Cast<AActor>(m_PreviewActor.GetObject())->GetActorLocation(), X, Y);
			FConEle& Ele = m_Grid[X][Y];
			if (!Ele.m_Foundation || Ele.m_Furniture)
				return false;
		}
		break;
	}
	return true;
}

IBuildable* UConstructionManager::SpawnStructure(const FBuildDataRow& data)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	IBuildable* StructActor = GetWorld()->SpawnActor<IBuildable>(data.m_ClassActor, Param);
	Cast<AActor>(StructActor)->SetActorScale3D(FVector(0.885f));
	StructActor->SetBuildData(data);

	return StructActor;
}

void UConstructionManager::ConfirmBuild()
{
	int X,Y;
	FVector Loc = Cast<AActor>(m_PreviewActor.GetObject())->GetActorLocation(); 

	switch (m_PreviewActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
		{
			GetIndex(Loc,X,Y);
			FConEle& Ele = m_Grid[X][Y];
			Ele.m_Foundation = m_PreviewActor; 
		}
		break;
	case EBuildType::Furniture:
		{
			GetIndex(Loc,X,Y);
			FConEle& Ele = m_Grid[X][Y];
			Ele.m_Furniture = m_PreviewActor; 
		}
		break;
	case EBuildType::Wall:
	case EBuildType::Door:
		{
			bool IsHori;
			GetWallIndex(Loc,X,Y,IsHori);
			if(IsHori)
				m_WallHorizontal[X].m_Walls[Y] = m_PreviewActor;
			else
				m_WallVertical[X].m_Walls[Y] = m_PreviewActor;
		}
		break;
	}
	
	const FBuildDataRow& BuildRow = m_PreviewActor->GetBuildData();
	
	m_PreviewActor->ConfirmBuild();

	if(Cast<IFocusable>(m_PreviewActor.GetObject()))
	{
		UMyGameInstance::Get->m_SpawnManager->AddFocusActor(m_PreviewActor.GetObject());
	}
	
	m_PreviewActor = nullptr;

	SpawnPreviewActor(Loc, &BuildRow);

	m_OnConfirm.Broadcast();
}

void UConstructionManager::Rotate()
{
	//float Yaw = FMath::RoundToFloat(m_PreviewActor->GetActorRotation().GetDenormalized().Yaw);
	if(m_PreviewActor)
	{
		Cast<AActor>(m_PreviewActor.GetObject())->AddActorLocalRotation(FRotator(0,90,0));
	}
	else if(m_FocusActor)
	{
		Cast<AActor>(m_FocusActor.GetObject())->AddActorLocalRotation(FRotator(0,90,0));	
	}
}

IBuildable* UConstructionManager::GetPreview()
{
	return (IBuildable*) m_PreviewActor.GetInterface();
}

void UConstructionManager::SelectStruct(IBuildable* sActor)
{
	m_FocusActor.SetInterface(sActor);
	m_FocusActor.SetObject(Cast<UObject>(sActor));
	m_FocusActor->ShowSelect(true);
}

void UConstructionManager::CancelSelect()
{
	if(m_FocusActor)
	{
		m_FocusActor->ShowSelect(false);
		m_FocusActor = nullptr;
	}
}

void UConstructionManager::Erase(IBuildable* buildActor)
{
	TScriptInterface<IBuildable> * Holder;
	bool isHori;
	GetStructureHolder(buildActor,Holder,isHori);

	if(Cast<IFocusable>((*Holder).GetObject()))
	{
		UMyGameInstance::Get->m_SpawnManager->RemoveFocusActor((*Holder).GetObject());
	}
	Cast<AActor>((*Holder).GetObject())->Destroy();
	(*Holder) = nullptr;
}

void UConstructionManager::Upgrade(IBuildable* buildActor)
{
	TScriptInterface<IBuildable> * Holder;
	bool isHori;
	GetStructureHolder(buildActor,Holder,isHori);
	AStructureActor* Structure = Cast<AStructureActor>((*Holder).GetObject()); 
	if(!Structure->TryUpgrade())
	{
		return;
	}
	FVector Loc = Structure->GetActorLocation();
	FName NextID = Structure->GetBuildData().m_NextUpgradeActorID;
	const FBuildDataRow* NextBuild = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(NextID,""); 
	AStructureActor* NewUpgradeActor = Cast<AStructureActor>(SpawnStructure(*NextBuild));
	NewUpgradeActor->SetActorLocation(Loc);
	if(!isHori)
	{
		NewUpgradeActor->SetActorRotation(FRotator(0,90,0));
	}
	NewUpgradeActor->ShowSelect(true);
	Structure->Destroy();
	(*Holder) = NewUpgradeActor;
	m_FocusActor = NewUpgradeActor;
}

void UConstructionManager::GetStructureHolder(IBuildable* want, TScriptInterface<IBuildable> *& holder, bool &isHori)
{
	FVector Loc =  Cast<AActor>(want)->GetActorLocation();

	int X,Y;

	switch (want->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
		GetIndex(Loc,X,Y);
		holder = &m_Grid[X][Y].m_Foundation;
		isHori = false;
		return;
	case EBuildType::Furniture:
		GetIndex(Loc,X,Y);
		holder = &m_Grid[X][Y].m_Furniture;
		isHori = false;
		return;
	case EBuildType::Wall:
	case EBuildType::Door:
		{
			GetWallIndex(Loc,X,Y,isHori);
			if(isHori)
				holder =  &m_WallHorizontal[X].m_Walls[Y];
			else
				holder =  &m_WallVertical[X].m_Walls[Y];
		}
		return;
	}
}

void UConstructionManager::Cancel()
{
	if(m_PreviewActor)
	{
		Cast<AActor>(m_PreviewActor.GetObject())->Destroy();
		m_PreviewActor = nullptr;
		m_OnCancel.Broadcast();
	}
}
