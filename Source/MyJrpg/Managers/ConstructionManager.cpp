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
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMat03(TEXT("Material'/Game/10_Construction/Materials/M_Hide.M_Hide'"));
	
	m_MatGreen = FoundMat01.Object;
	m_MatRed = FoundMat02.Object;
	m_MatCyan = FoundMat03.Object;
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
	
	m_WallHorizontal.Init(FWallAry(FGlobalVariable::GRID_COUNT + 1),FGlobalVariable::GRID_COUNT);//y x
	m_WallVertical.Init(FWallAry(FGlobalVariable::GRID_COUNT),FGlobalVariable::GRID_COUNT + 1);
	
	EndBuilding();
}

void UConstructionManager::StartBuilding()
{
	m_GridMesh->Show();

	SetFurnitureHide();
}

void UConstructionManager::EndBuilding()
{
	m_GridMesh->Hide();
	Cancel();
	CancelSelect();
	SetFurnitureWallShow();
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
			if(Ele.m_Foundation.Get())
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
			if(Ele.m_Furniture.Get())
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
				if((*AryWall)[x].m_Walls[y].Get())
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
				if ((*AryWall)[x].m_Walls[y].Get())
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
	case EBuildType::Field:
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
		if(m_PreviewActor.Get())
		{
			dataRow = &m_PreviewActor->GetBuildData();
		}
		else
			return;
	}
	FVector NewLoc;

	FRotator NewRot;

	GetEmptyLoc(loc,NewLoc, NewRot, dataRow->m_BuildType);

	if (!m_PreviewActor.Get() || &m_PreviewActor->GetBuildData() != dataRow)
	{
		if(m_PreviewActor.Get())
		{
			m_PreviewActor->Destroy();
			m_PreviewActor = nullptr;
		}
		m_PreviewActor = SpawnStructure(*dataRow);
		m_PreviewActor->SetActorEnableCollision(false);
	}
	
	m_PreviewActor->SetActorLocation(NewLoc + dataRow->m_Offset);
	m_PreviewActor->SetActorRotation(NewRot);

	CheckBuildable();
}

bool UConstructionManager::IsBuildable()
{
	if(!m_PreviewActor.Get())
		return false;
	
	if(!UMyGameInstance::Get->m_CraftManager->IsMaterialEnough(m_PreviewActor->GetBuildData().m_AryCostItem))
		return false;
	
	int X,Y;

	FVector Loc = m_PreviewActor->GetActorLocation();
	
	switch (m_PreviewActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
	case EBuildType::Field:
		{
			GetIndex(Loc , X, Y);
			FConEle& Ele = m_Grid[X][Y];
			if (Ele.m_Foundation.Get())
				return false;
			if(!TraceBuildable(Loc,FVector(170.f,170.f,10),FRotator::ZeroRotator, 50))
				return false;
		}
			break;
	case EBuildType::Wall:
		{
			bool IsHori;
			GetWallIndex(Loc, X, Y, IsHori);
			if (IsHori)
			{
				if (!m_Grid[X][Y - 1].m_Foundation.Get() && !m_Grid[X][Y].m_Foundation.Get())
					return false;
				if (m_WallHorizontal[X].m_Walls[Y].Get())
					return false;
			}
			else
			{
				if (!m_Grid[X - 1][Y].m_Foundation.Get() && !m_Grid[X][Y].m_Foundation.Get())
					return false;
				if (m_WallVertical[X].m_Walls[Y].Get())
					return false;
			}
			if(!TraceBuildable(Loc,FVector(120,30,10),IsHori ? FRotator(0,90,0) : FRotator::ZeroRotator, 75))
				return false;
		}
		break;
	case EBuildType::Furniture:
		{
			GetIndex( Loc, X, Y);
			FConEle& Ele = m_Grid[X][Y];
			if (!Ele.m_Foundation.Get() || Ele.m_Furniture.Get())
				return false;
			if(Ele.m_Foundation->GetBuildData().m_BuildType == EBuildType::Field)
				return false;
			if(!TraceBuildable(Loc,FVector(120,120,10),FRotator::ZeroRotator, 100))
				return false;
		}
		break;
	}

	return true; 
}

bool UConstructionManager::TraceBuildable(const FVector& Loc, const FVector&& extent, const FRotator& rot, float height)
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> AryIgnore;

	FHitResult Hit;

	FVector Dest = Loc + FVector(0,0,height);
	
	if(UKismetSystemLibrary::BoxTraceSingleForObjects(this,Dest,Dest,extent,
		rot,ObjectTypes,false,AryIgnore,EDrawDebugTrace::Persistent,Hit,false))
	{
		return false;
	}
	
	return true;
}

void UConstructionManager::SetFurnitureWallShow()
{
	int Iter = 0;
	int IterY = 0;

	while (Iter < FGlobalVariable::GRID_COUNT)
	{
		while (IterY < FGlobalVariable::GRID_COUNT)
		{
			FConEle& Grid = m_Grid[Iter][IterY]; 
			if(Grid.m_Furniture.Get())
			{
				Grid.m_Furniture->SetMat(nullptr);
				Grid.m_Furniture->SetColl(true);
			}
			if(Grid.m_Foundation.Get())
			{
				Grid.m_Foundation->SetMat(nullptr);
				Grid.m_Foundation->SetColl(true);
			}
			TWeakObjectPtr<AStructureActor> HoriWall = m_WallHorizontal[Iter].m_Walls[IterY]; 
			if(HoriWall.Get())
			{
				HoriWall->SetMat(nullptr);
				HoriWall->SetColl(true);
			}
			TWeakObjectPtr<AStructureActor> VertWall = m_WallVertical[Iter].m_Walls[IterY];
			if(VertWall.Get())
			{
				VertWall->SetMat(nullptr);
				VertWall->SetColl(true);
			}
			IterY++;
		}
		IterY = 0;
		Iter++;
	}
}

void UConstructionManager::SetFurnitureHide()
{
	int Iter = 0;
	int IterY = 0;

	while (Iter < FGlobalVariable::GRID_COUNT)
	{
		while (IterY < FGlobalVariable::GRID_COUNT)
		{
			FConEle& Grid = m_Grid[Iter][IterY]; 
			if(Grid.m_Furniture.Get())
			{
				Grid.m_Furniture->SetMat(m_MatCyan);
				Grid.m_Furniture->SetColl(false);
			}
			if(Grid.m_Foundation.Get())
			{
				if(Grid.m_Foundation.Get()->GetBuildData().m_BuildType != EBuildType::Field)
				{
					Grid.m_Foundation->SetMat(nullptr);
					Grid.m_Foundation->SetColl(true);
				}
				else
				{
					Grid.m_Foundation->SetMat(m_MatCyan);
					Grid.m_Foundation->SetColl(false);
				}
			}
			TWeakObjectPtr<AStructureActor> HoriWall = m_WallHorizontal[Iter].m_Walls[IterY]; 
			if(HoriWall.Get())
			{
				HoriWall->SetMat(nullptr);
				HoriWall->SetColl(true);
			}
			TWeakObjectPtr<AStructureActor> VertWall = m_WallVertical[Iter].m_Walls[IterY];
			if(VertWall.Get())
			{
				VertWall->SetMat(nullptr);
				VertWall->SetColl(true);
			}
			IterY++;
		}
		IterY = 0;
		Iter++;
	}
}

void UConstructionManager::SetWallStructureHide()
{
	int Iter = 0;
	int IterY = 0;

	while (Iter < FGlobalVariable::GRID_COUNT)
	{
		while (IterY < FGlobalVariable::GRID_COUNT)
		{
			FConEle& Grid = m_Grid[Iter][IterY];
			if (Grid.m_Furniture.Get())
			{
				Grid.m_Furniture->SetMat(nullptr);
				Grid.m_Furniture->SetColl(true);
			}
			if (Grid.m_Foundation.Get())
			{
				if (Grid.m_Foundation.Get()->GetBuildData().m_BuildType != EBuildType::Field)
				{
					Grid.m_Foundation->SetMat(m_MatCyan);
					Grid.m_Foundation->SetColl(false);
				}
				else
				{
					Grid.m_Foundation->SetMat(nullptr);
					Grid.m_Foundation->SetColl(true);
				}
			}
			TWeakObjectPtr<AStructureActor> HoriWall = m_WallHorizontal[Iter].m_Walls[IterY];
			if (HoriWall.Get())
			{
				HoriWall->SetMat(m_MatCyan);
				HoriWall->SetColl(false);
			}
			TWeakObjectPtr<AStructureActor> VertWall = m_WallVertical[Iter].m_Walls[IterY];
			if (VertWall.Get())
			{
				VertWall->SetMat(m_MatCyan);
				VertWall->SetColl(false);
			}
			IterY++;
		}
		IterY = 0;
		Iter++;
	}
}

AStructureActor* UConstructionManager::SpawnStructure(const FBuildDataRow& data)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStructureActor* StructActor = GetWorld()->SpawnActor<AStructureActor>(data.m_ClassActor, Param);
	Cast<AActor>(StructActor)->SetActorScale3D(FVector(0.885f));
	StructActor->SetBuildData(data);

	return StructActor;
}

bool UConstructionManager::IsEraseable()
{
	switch (m_FocusActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
		{
			int X,Y;
			FVector Loc = m_FocusActor->GetActorLocation();		
			GetIndex(Loc,X,Y);
			return !m_Grid[X][Y].m_Furniture.Get();
		}
	case EBuildType::Field:
	case EBuildType::Furniture:
		return m_FocusActor->IsEraseable();
	}
	return true;
}

void UConstructionManager::OnErase(const FVector& Loc)
{
	int X,Y;
	
	GetIndex(Loc,X,Y);
	
	if(0 > Y - 1 || !m_Grid[X][Y - 1].m_Foundation.Get())
	{
		TWeakObjectPtr<AStructureActor>& Holder = m_WallHorizontal[X].m_Walls[Y];
		TryEraseActor(Holder);
	}
	if(FGlobalVariable::GRID_COUNT <= Y + 1 || !m_Grid[X][Y + 1].m_Foundation.Get())//없다면 인데 최대 인덱스를 초과해서 똥값확인중,인덱스 초과시 어떻게?없는것으로 처줘야함
	{
		TWeakObjectPtr<AStructureActor>& Holder = m_WallHorizontal[X].m_Walls[Y + 1];//n
		TryEraseActor(Holder);
	}
	if(0 > X - 1 || !m_Grid[X - 1][Y].m_Foundation.Get())
	{
		TWeakObjectPtr<AStructureActor>& Holder = m_WallVertical[X].m_Walls[Y];//w
		TryEraseActor(Holder);
	}
	if(FGlobalVariable::GRID_COUNT <= X + 1 && !m_Grid[X + 1][Y].m_Foundation.Get())//FGlobalVariable::GRID_COUNT > X + 1 
	{
		TWeakObjectPtr<AStructureActor>& Holder = m_WallVertical[X + 1].m_Walls[Y];//e
		TryEraseActor(Holder);
	}
}

void UConstructionManager::TryEraseActor(TWeakObjectPtr<AStructureActor>& holder)
{
	if(!holder.Get())
		return;
	holder->Destroy();
	holder = nullptr;
}

void UConstructionManager::ConfirmBuild()
{
	UMyGameInstance::Get->m_CraftManager->PurchaseItemForCraft(m_PreviewActor->GetBuildData().m_AryCostItem);
	
	int X,Y;
	FVector Loc = m_PreviewActor->GetActorLocation(); 

	switch (m_PreviewActor->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
	case EBuildType::Field:
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
	if(BuildRow.m_BuildType == EBuildType::Furniture)
	{
		RemoveFurniture(m_PreviewActor->GetBuildData().m_RowID);
	}
	m_PreviewActor->ConfirmBuild();

	IFocusable* Focus = Cast<IFocusable>(m_PreviewActor.Get());

	if(Focus && Focus->IsInteractImplemented())
	{
		UMyGameInstance::Get->m_SpawnManager->AddFocusActor(m_PreviewActor.Get());
	}
	
	m_PreviewActor = nullptr;

	SpawnPreviewActor(Loc, &BuildRow);

	m_OnConfirm.Broadcast();
}

void UConstructionManager::Rotate()
{
	if(m_PreviewActor.Get())
	{
		m_PreviewActor->AddActorLocalRotation(FRotator(0,90,0));
	}
	else if(m_FocusActor.Get())
	{
		m_FocusActor->AddActorLocalRotation(FRotator(0,90,0));	
	}
}

AStructureActor* UConstructionManager::GetPreview()
{
	return m_PreviewActor.Get();
}

void UConstructionManager::SelectStruct(AStructureActor* sActor)
{
	m_FocusActor = sActor;
	m_FocusActor->ShowSelect(true);
}

void UConstructionManager::Erase(AStructureActor* buildActor)
{
	if(!IsEraseable())
	{
		return;
	}
	TWeakObjectPtr<AStructureActor> * Holder;
	bool isHori;
	GetStructureHolder(buildActor,Holder,isHori);

	if(Cast<IFocusable>((*Holder).Get()))
	{
		UMyGameInstance::Get->m_SpawnManager->RemoveFocusActor((*Holder).Get());
	}
	if(buildActor->GetBuildData().m_BuildType == EBuildType::Foundation)
	{
		FVector Loc = (*Holder)->GetActorLocation(); 
		OnErase(Loc);
	}
	else if (buildActor->GetBuildData().m_BuildType == EBuildType::Furniture)
	{
		AddFurniture(buildActor->GetBuildData().m_RowID);
	}
	TryEraseActor(*Holder);
}

void UConstructionManager::Upgrade(AStructureActor* buildActor)
{
	TWeakObjectPtr<AStructureActor> * Holder;
	bool isHori;
	GetStructureHolder(buildActor,Holder,isHori);
	AStructureActor* Structure = Holder->Get(); 
	if(!Structure->IsUpgradeable())
		return;
	FVector Loc = Structure->GetActorLocation();
	FName NextID = Structure->GetBuildData().m_NextUpgradeActorID;
	const FBuildDataRow* NextBuild = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(NextID,""); 
	AStructureActor* NewUpgradeActor = Cast<AStructureActor>(SpawnStructure(*NextBuild));
	NewUpgradeActor->SetActorLocation(Loc);
	if(!isHori)
		NewUpgradeActor->SetActorRotation(FRotator(0,90,0));
	NewUpgradeActor->ShowSelect(true);
	Structure->Destroy();
	(*Holder) = NewUpgradeActor;
	m_FocusActor = NewUpgradeActor;
}

void UConstructionManager::GetStructureHolder(AStructureActor* want, TWeakObjectPtr<AStructureActor> *& holder, bool &isHori)
{
	FVector Loc =  Cast<AActor>(want)->GetActorLocation();

	int X,Y;

	switch (want->GetBuildData().m_BuildType)
	{
	case EBuildType::Foundation:
	case EBuildType::Field:
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

void UConstructionManager::AddFurniture(const FName& id)
{
	int* Count = m_MapInvenFurniture.Find(id);
	if(Count)
	{
		(*Count)++;
	}
	else
	{
		m_MapInvenFurniture.Add(id, 1);
	}

	m_OnChanged.Broadcast();
}

void UConstructionManager::RemoveFurniture(const FName& id, int amount)
{
	int* Count = m_MapInvenFurniture.Find(id);
	if(Count)
	{
		(*Count)-=amount;
		if((*Count) < 1)
		{
			m_MapInvenFurniture.Remove(id);
		}
	}
	m_OnChanged.Broadcast();
}

const TMap<FName, int>& UConstructionManager::GetInvenFurniture() const
{
	return m_MapInvenFurniture;
}

bool UConstructionManager::HasFurnitureEmptySpace(FName id)
{
	int* pCnt = m_MapInvenFurniture.Find(id);
	if(!pCnt)
	{
		return true;
	}
	int Max = GetFurnitureMaxOwnCnt(id);

	if(Max == 0 || Max > *pCnt)
	{
		return true;
	}
	return false;
}

int UConstructionManager::GetFurnitureMaxOwnCnt(FName id)
{
	const FBuildDataRow* BuildData = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(id, "UConstructionManager::GetFurnitureMaxOwnCnt No ID?");

	return BuildData->m_nMaxOwnedCount;	
}

void UConstructionManager::Cancel()
{
	if(m_PreviewActor.Get())
	{
		m_PreviewActor->Destroy();
		m_PreviewActor = nullptr;
		m_OnCancel.Broadcast();
	}
}

void UConstructionManager::CancelSelect()
{
	if(m_FocusActor.Get())
	{
		m_FocusActor->ShowSelect(false);
		m_FocusActor = nullptr;
	}
}
