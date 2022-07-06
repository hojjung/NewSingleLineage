#include "NPCPaletteEditorEdMode.h"

#include "EditorModeManager.h"
#include "EditorViewportClient.h"
#include "Modules/ModuleManager.h"
#include "EditorModes.h"
#include "LevelEditor.h"
#include "Engine/Selection.h"
#include "ScopedTransaction.h"
#include "Toolkits/ToolkitManager.h"
#include "IAssetViewport.h"
#include "INPCPaletteEditorModule.h"
#include "NPCPaletteEditorEdModeToolkit.h"
#include "NPCPaletteGizmoActor.h"
#include "NPCPaletteGizmoActorFactory.h"
#include "Components/BillboardComponent.h"

FNPCPaletteEdMode::FNPCPaletteEdMode()
	: AssetsToPlace()
	  , ActiveTransactionIndex(INDEX_NONE)
	  , PlacementsChanged(false)
	  , CreatedPreviewActors(false)
{
	m_CurrentAsset=nullptr;
}

FNPCPaletteEdMode::~FNPCPaletteEdMode()
{
}

const FEditorModeID FNPCPaletteEdMode::EM_NPCPaletteEdModeId(TEXT("NPCPaletteEdModeID"));

void FNPCPaletteEdMode::Initialize()
{
	//삽입해봄


	//, const TArray<UObject*>&, const TArray<AActor*>&);
}

bool FNPCPaletteEdMode::UsesToolkits() const
{
	return true;
}

void FNPCPaletteEdMode::Enter()
{
	// Call parent implementation
	FEdMode::Enter();

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FNPCPaletteEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
	SpawnPlacedActor();

	BindDelegate();
}

void FNPCPaletteEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	ClearPlacedAssetActor();

	UnbindDelegate();

	ClearAssetsToPlace();

	// Call parent implementation
	m_CurrentAsset = nullptr;
	FEdMode::Exit();
}

void FNPCPaletteEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	if (IsCurrentlyPlacing())
	{
		if (ViewportClient)
		{
			ViewportClient->SetRequiredCursorOverride(true, EMouseCursor::GrabHandClosed);
		}

		bool HasValidFocusTarget = false;
		for (int Index = ValidFocusTargetsForPlacement.Num() - 1; !HasValidFocusTarget && Index >= 0; Index--)
		{
			TSharedPtr<SWidget> FocusTarget = ValidFocusTargetsForPlacement[Index].Pin();

			if (FocusTarget.IsValid())
			{
				if (FocusTarget->HasKeyboardFocus() || FocusTarget->HasFocusedDescendants())
				{
					HasValidFocusTarget = true;
				}
			}
			else
			{
				ValidFocusTargetsForPlacement.RemoveAt(Index);
			}
		}

		if (!HasValidFocusTarget)
		{
			FLevelEditorModule* LevelEditorModule = FModuleManager::Get().LoadModulePtr<FLevelEditorModule>(
				"LevelEditor");
			if (LevelEditorModule != NULL)
			{
				TSharedPtr<ILevelEditor> LevelEditor = LevelEditorModule->GetFirstLevelEditor();
				const TArray<TSharedPtr<IAssetViewport>>& LevelViewports = LevelEditor->GetViewports();
				for (auto It(LevelViewports.CreateConstIterator()); !HasValidFocusTarget && It; It++)
				{
					const TSharedPtr<IAssetViewport>& Viewport = *It;
					const TSharedPtr<const SWidget> ViewportWidget = Viewport->AsWidget();
					HasValidFocusTarget = ViewportWidget->HasKeyboardFocus() || ViewportWidget->HasFocusedDescendants();
				}
			}
		}

		if (!HasValidFocusTarget)
		{
			StopPlacing();
		}
	}
	else if (ViewportClient != NULL)
	{
		ViewportClient->SetRequiredCursorOverride(false);
	}

	if (CreatedPreviewActors && ViewportClient != NULL && PlacementsChanged)
	{
		ViewportClient->DestroyDropPreviewActors();
		PlacementsChanged = false;
		CreatedPreviewActors = false;
	}

	FEdMode::Tick(ViewportClient, DeltaTime);
}

bool FNPCPaletteEdMode::MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	if (IsCurrentlyPlacing())
	{
		ViewportClient->SetRequiredCursorOverride(true, EMouseCursor::GrabHandClosed);
	}

	return FEdMode::MouseEnter(ViewportClient, Viewport, x, y);
}

bool FNPCPaletteEdMode::MouseLeave(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
	if (!ViewportClient->IsTracking())
	{
		ViewportClient->SetRequiredCursorOverride(false);
		ViewportClient->DestroyDropPreviewActors();
		CreatedPreviewActors = false;
	}

	return FEdMode::MouseLeave(ViewportClient, Viewport);
}

bool FNPCPaletteEdMode::AllowPreviewActors(FEditorViewportClient* ViewportClient) const
{
	return (IsCurrentlyPlacing() && (ViewportClient->IsTracking() && AllowPreviewActorsWhileTracking)) || !
		ViewportClient->IsTracking();
}

bool FNPCPaletteEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	UpdatePreviewActors(ViewportClient, Viewport, x, y);
	return FEdMode::MouseMove(ViewportClient, Viewport, x, y);
}

void FNPCPaletteEdMode::UpdatePreviewActors(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x,
                                            int32 y)
{
	bool AllAssetsValid = false;
	bool AllAssetsCanBeDropped = false;

	if (PlacementsChanged)
	{
		ViewportClient->DestroyDropPreviewActors();
		PlacementsChanged = false;
		CreatedPreviewActors = false;
	}

	const bool bAllowPreviewActors = AllowPreviewActors(ViewportClient);

	if (bAllowPreviewActors && AssetsToPlace.Num() > 0)
	{
		AllAssetsValid = true;
		TArray<UObject*> Assets;
		for (int Index = 0; Index < AssetsToPlace.Num(); Index++)
		{
			if (AssetsToPlace[Index].IsValid())
			{
				Assets.Add(AssetsToPlace[Index].Get());
			}
			else
			{
				ViewportClient->DestroyDropPreviewActors();
				AllAssetsValid = false;
				CreatedPreviewActors = false;
				break;
			}
		}

		if (AllAssetsValid)
		{
			AllAssetsCanBeDropped = true;
			// Determine if we can drop the assets
			for (auto AssetIt = Assets.CreateConstIterator(); AssetIt; ++AssetIt)
			{
				UObject* Asset = *AssetIt;

				FDropQuery DropResult = ViewportClient->CanDropObjectsAtCoordinates(x, y, FAssetData(Asset));
				if (!DropResult.bCanDrop)
				{
					// At least one of the assets can't be dropped.
					ViewportClient->DestroyDropPreviewActors();
					AllAssetsCanBeDropped = false;
					CreatedPreviewActors = false;
				}
			}

			if (AllAssetsCanBeDropped)
			{
				// Update the currently dragged actor if it exists
				bool bDroppedObjectsVisible = true;
				if (!ViewportClient->UpdateDropPreviewActors(x, y, Assets, bDroppedObjectsVisible,
				                                             PlacementFactory.Get()))
				{
					const bool bOnlyDropOnTarget = false;
					const bool bCreateDropPreview = true;
					const bool bSelectActors = false;
					TArray<AActor*> TemporaryActors;
					CreatedPreviewActors = ViewportClient->DropObjectsAtCoordinates(
						x, y, Assets, TemporaryActors, bOnlyDropOnTarget, bCreateDropPreview, bSelectActors,
						PlacementFactory.Get());
				}
			}
			else
			{
				StopPlacing();
			}
		}
	}

	if (!bAllowPreviewActors || !AllAssetsValid || !AllAssetsCanBeDropped)
	{
		ViewportClient->DestroyDropPreviewActors();
		CreatedPreviewActors = false;
	}
}

void FNPCPaletteEdMode::SelectDropEntity(const FNPCPaletteItem& item)
{
	m_DropNpcItem = &item;
}

void FNPCPaletteEdMode::OnSuccessActorPlaced(const FNPCPaletteItem& currentItem,AActor* actorPlaced)
{
	ANPCPaletteGizmoActor* GizmoAcotr = Cast<ANPCPaletteGizmoActor>(actorPlaced);

	GizmoAcotr->m_RowID = currentItem.m_RowID;

	GizmoAcotr->m_CurrentNPC = currentItem;
	
	GizmoAcotr->SetTextIcon(GetAssetName(currentItem.m_SelectedEntityRow), GetAssetIcon(currentItem.m_SelectedEntityRow));

	m_MyPlacedActors.Add(GizmoAcotr);
}

UTexture2D* FNPCPaletteEdMode::GetAssetIcon(FEntityRow* row)
{
	return row->m_Icon.LoadSynchronous();
}

FText FNPCPaletteEdMode::GetAssetName(FEntityRow* row)
{
	return row->m_ShowingName;
}

void FNPCPaletteEdMode::ClearPlacedAssetActor()
{
	bool bNeedSaveActorData = m_MyPlacedActors.Num() > 0;

	bool bSaveAssetSet = m_CurrentAsset.Get() != nullptr;

	if (bSaveAssetSet)
	{
		m_CurrentAsset->m_ArySpawnDatas.Reset();

		for (auto& actorIter : m_MyPlacedActors)
		{
			if(actorIter->IsPendingKillPending())
			{
				continue;
			}
			
			if (m_CurrentAsset.Get())
			{
				FNPCSpawnData Data;

				ANPCPaletteGizmoActor* GizmoActor = Cast<ANPCPaletteGizmoActor>(actorIter);

				if(!GizmoActor)
				{
					continue;
				}

				Data.m_IDEntity = GizmoActor->m_RowID;

				Data.m_SpawnPosition = actorIter->GetActorLocation();

				Data.m_SpawnRotation = actorIter->GetActorRotation();

				Data.m_EntityParentTable = GizmoActor->m_CurrentNPC.m_EntityParentTable;

				m_CurrentAsset->m_ArySpawnDatas.Add(Data);
			}

			actorIter->Destroy();
		}
	}
	else
	{
		for (auto& actorIter : m_MyPlacedActors)
		{
			if (actorIter)
			{
				actorIter->Destroy();
			}
		}
	}

	if (bNeedSaveActorData && bSaveAssetSet)
	{
		m_CurrentAsset->Modify();
	}

	m_MyPlacedActors.Reset();
}

void FNPCPaletteEdMode::SpawnPlacedActor()
{
	if (!m_CurrentAsset.Get())
	{
		return;
	}

	for (FNPCSpawnData& Data : m_CurrentAsset->m_ArySpawnDatas)
	{
		//Load Data with FName
		FActorSpawnParameters Param;

		Param.bNoFail = true;

		ANPCPaletteGizmoActor* ActorSpawned = GetWorld()->SpawnActor<ANPCPaletteGizmoActor>(
			Data.m_SpawnPosition, Data.m_SpawnRotation, Param);

		check(Data.m_EntityParentTable);

		FNPCPaletteItem NPCItem;

		if(!CreateNPCItemStruct(Data,NPCItem))
		{
			continue;//실패
		}
		
		OnSuccessActorPlaced(NPCItem,ActorSpawned);
	}
}

void FNPCPaletteEdMode::SetNPCAsset(UNPCPaletteDataAsset* assetNew)
{
	//m_CurrentAsset = MakeShared<UNPCPaletteDataAsset>(assetNew);

	if (m_CurrentAsset.Get())
	{
		ClearPlacedAssetActor();
		ClearAssetsToPlace();
	}

	m_CurrentAsset = assetNew;

	SpawnPlacedActor();
}

const UNPCPaletteDataAsset* FNPCPaletteEdMode::GetNPCAsset() const
{
	return m_CurrentAsset.Get();
}

void FNPCPaletteEdMode::BindDelegate()
{
	m_DeleHandle = FEditorDelegates::OnAssetDragStarted.AddRaw(this, &FNPCPaletteEdMode::StartPlacing);

	//FEditorDelegates::OnDeleteActorsEnd
}

void FNPCPaletteEdMode::UnbindDelegate()
{
	FEditorDelegates::OnAssetDragStarted.Remove(m_DeleHandle);

	m_DeleHandle.Reset();
}

bool FNPCPaletteEdMode::CreateNPCItemStruct(const FNPCSpawnData& dataIn,FNPCPaletteItem& outItem)
{
	FEntityRow* Row = dataIn.m_EntityParentTable->FindRow<FEntityRow>(dataIn.m_IDEntity,"");

	if(!Row)
	{
		return false;
	}
	
	outItem = FNPCPaletteItem(dataIn.m_IDEntity,Row,dataIn.m_EntityParentTable,*UNPCPaletteGizmoActorFactory::StaticClass(),FAssetData(ANPCPaletteGizmoActor::StaticClass()));

	return true;
}


bool FNPCPaletteEdMode::InputKey(FEditorViewportClient* InViewportClient, FViewport* InViewport, FKey InKey,
                                 EInputEvent InEvent)
{
	bool Handled = false;
	const bool bIsCtrlDown = ((InKey == EKeys::LeftControl || InKey == EKeys::RightControl) && InEvent != IE_Released)
		|| InViewport->KeyState(EKeys::LeftControl) || InViewport->KeyState(EKeys::RightControl);

	if (IsCurrentlyPlacing())
	{
		if (InEvent == EInputEvent::IE_Pressed && (InKey == EKeys::Escape || InKey == EKeys::SpaceBar))
		{
			StopPlacing();
			Handled = true;
		}
		else if (bIsCtrlDown)
		{
			AllowPreviewActorsWhileTracking = true;
		}
		else if (!bIsCtrlDown)
		{
			AllowPreviewActorsWhileTracking = false;

			if (PlacedActors.Num() > 0)
			{
				SelectPlacedActors();

				ClearAssetsToPlace();
				INPCPaletteEditorModule::Get().BroadcastStoppedPlacing(true);
			}
		}
	}

	if (Handled)
	{
		return true;
	}

	return FEdMode::InputKey(InViewportClient, InViewport, InKey, InEvent);
}

bool FNPCPaletteEdMode::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	PlacedActorsThisTrackingSession = false;

	if (IsCurrentlyPlacing() && ActiveTransactionIndex == INDEX_NONE)
	{
		InViewportClient->SetRequiredCursorOverride(true, EMouseCursor::GrabHandClosed);
		ActiveTransactionIndex = GEditor->BeginTransaction(NSLOCTEXT("BuilderMode", "PlaceActor", "Placed Actor"));
		return true;
	}

	return FEdMode::StartTracking(InViewportClient, InViewport);
}

bool FNPCPaletteEdMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	if (IsCurrentlyPlacing())
	{
		InViewportClient->SetRequiredCursorOverride(true, EMouseCursor::GrabHandClosed);
	}

	if (ActiveTransactionIndex != INDEX_NONE)
	{
		if (!PlacedActorsThisTrackingSession)
		{
			GEditor->CancelTransaction(ActiveTransactionIndex);
			ActiveTransactionIndex = INDEX_NONE;
		}
		else
		{
			GEditor->EndTransaction();
			ActiveTransactionIndex = INDEX_NONE;
		}
		return true;
	}

	return FEdMode::EndTracking(InViewportClient, InViewport);
}

bool FNPCPaletteEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy,
                                    const FViewportClick& Click)
{
	bool Handled = false;

	if (IsCurrentlyPlacing())
	{
		if (Click.GetKey() == EKeys::LeftMouseButton)
		{
			TArray<UObject*> Assets;
			for (int Index = 0; Index < AssetsToPlace.Num(); Index++)
			{
				if (AssetsToPlace[Index].IsValid())
				{
					Assets.Add(AssetsToPlace[Index].Get());
				}
			}

			TArray<AActor*> OutNewActors;
			const bool bCreateDropPreview = false;
			const bool SelectActor = false;
			const FViewport* const Viewport = Click.GetViewportClient()->Viewport;

			bool AllAssetsCanBeDropped = true;
			// Determine if we can drop the assets
			for (auto AssetIt = Assets.CreateConstIterator(); AssetIt; ++AssetIt)
			{
				UObject* Asset = *AssetIt;
				FDropQuery DropResult = InViewportClient->CanDropObjectsAtCoordinates(
					Viewport->GetMouseX(), Viewport->GetMouseY(), FAssetData(Asset));
				if (!DropResult.bCanDrop)
				{
					// At least one of the assets can't be dropped.
					InViewportClient->DestroyDropPreviewActors();
					AllAssetsCanBeDropped = false;
					CreatedPreviewActors = false;
				}
			}

			if (AllAssetsCanBeDropped)
			{
				if (!Click.IsControlDown())
				{
					ClearAssetsToPlace();
					INPCPaletteEditorModule::Get().BroadcastStoppedPlacing(true);
					InViewportClient->SetRequiredCursorOverride(true, EMouseCursor::GrabHand);
				}

				InViewportClient->DropObjectsAtCoordinates(Viewport->GetMouseX(), Viewport->GetMouseY(), Assets,
				                                           OutNewActors, false, bCreateDropPreview, SelectActor,
				                                           PlacementFactory.Get());

				for (int Index = 0; Index < OutNewActors.Num(); Index++)
				{
					if (OutNewActors[Index] != NULL)
					{
						PlacedActorsThisTrackingSession = true;
						
						PlacedActors.Add(OutNewActors[Index]);
						OnSuccessActorPlaced(*m_DropNpcItem,OutNewActors[Index]);
					}
				}

				if (!Click.IsControlDown())
				{
					SelectPlacedActors();
					ClearAssetsToPlace();
				}

				Handled = true;
			}
		}
		else
		{
			InViewportClient->DestroyDropPreviewActors();
			CreatedPreviewActors = false;
			StopPlacing();
		}
	}

	if (!Handled)
	{
		Handled = FEdMode::HandleClick(InViewportClient, HitProxy, Click);
	}

	return Handled;
}

bool FNPCPaletteEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag,
                                   FRotator& InRot, FVector& InScale)
{
	if (IsCurrentlyPlacing())
	{
		const bool bIsCtrlDown = InViewport->KeyState(EKeys::LeftControl) || InViewport->KeyState(EKeys::RightControl);

		if (InViewport->KeyState(EKeys::MiddleMouseButton))
		{
			StopPlacing();

			InViewportClient->DestroyDropPreviewActors();
			CreatedPreviewActors = false;
		}
		else if (InViewport->KeyState(EKeys::RightMouseButton))
		{
			if (bIsCtrlDown)
			{
				StopPlacing();
			}

			InViewportClient->DestroyDropPreviewActors();
			CreatedPreviewActors = false;
		}
		else if (InViewport->KeyState(EKeys::LeftMouseButton))
		{
			if (!bIsCtrlDown)
			{
				InViewportClient->DestroyDropPreviewActors();
				CreatedPreviewActors = false;
			}
			else
			{
				return true;
			}
		}
	}

	return FEdMode::InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
}

bool FNPCPaletteEdMode::ShouldDrawWidget() const
{
	return IsCurrentlyPlacing() ? false : FEdMode::ShouldDrawWidget();
}

bool FNPCPaletteEdMode::UsesPropertyWidgets() const
{
	return IsCurrentlyPlacing() ? false : FEdMode::ShouldDrawWidget();
}

bool FNPCPaletteEdMode::IsCompatibleWith(FEditorModeID OtherModeID) const
{
	return
		OtherModeID == FBuiltinEditorModes::EM_InterpEdit ||
		OtherModeID == FBuiltinEditorModes::EM_MeshPaint ||
		OtherModeID == FBuiltinEditorModes::EM_Foliage ||
		OtherModeID == FBuiltinEditorModes::EM_Level ||
		OtherModeID == FBuiltinEditorModes::EM_Physics ||
		OtherModeID == FBuiltinEditorModes::EM_ActorPicker ||
		OtherModeID == FNPCPaletteEdMode::EM_NPCPaletteEdModeId;
}

void FNPCPaletteEdMode::ActorsDuplicatedNotify(TArray<AActor*>& PreDuplicateSelection,
	TArray<AActor*>& PostDuplicateSelection, bool bOffsetLocations)
{
	FEdMode::ActorsDuplicatedNotify(PreDuplicateSelection, PostDuplicateSelection, bOffsetLocations);

	int Iter = 0;

	while (Iter < PreDuplicateSelection.Num())
	{
		ANPCPaletteGizmoActor* GizmoAcotr = Cast<ANPCPaletteGizmoActor>(PreDuplicateSelection[Iter]);
		if(!GizmoAcotr)
		{
			continue;
		}
		OnSuccessActorPlaced(GizmoAcotr->m_CurrentNPC ,PostDuplicateSelection[Iter]);
		Iter++;
	}
}

void FNPCPaletteEdMode::PostUndo()
{
	FEdMode::PostUndo();
}

void FNPCPaletteEdMode::StartPlacing(const TArray<FAssetData>& Assets, UActorFactory* Factory)
{
	if(!GetNPCAsset())
	{
		return;
	}
	const bool bNotifySelectNone = true;
	const bool bDeselectBSPSurfs = true;
	GEditor->SelectNone(bNotifySelectNone, bDeselectBSPSurfs);

	if (Assets.Num() == 1)
	{
		if (Assets[0] != NULL)
		{
			AssetsToPlace.Add(Assets[0].GetAsset());
			PlacementsChanged = true;

			if (Factory == NULL)
			{
				UActorFactory* LastSetFactory = FindLastUsedFactoryForAssetType(Assets[0].GetAsset());
				if (LastSetFactory != NULL)
				{
					Factory = LastSetFactory;
				}
			}

			SetPlacingFactory(Factory);
		}
	}
	else
	{
		for (int Index = 0; Index < Assets.Num(); Index++)
		{
			if (Assets[Index] != NULL)
			{
				AssetsToPlace.Add(Assets[Index].GetAsset());
				PlacementsChanged = true;
			}
		}

		if (PlacementsChanged == true)
		{
			SetPlacingFactory(Factory);
		}
	}

	INPCPaletteEditorModule::Get().BroadcastStartedPlacing(Assets);
}

void FNPCPaletteEdMode::StopPlacing()
{
	if (IsCurrentlyPlacing())
	{
		ClearAssetsToPlace();
		INPCPaletteEditorModule::Get().BroadcastStoppedPlacing(false);
		PlacementsChanged = true;
	}
}

void FNPCPaletteEdMode::SetPlacingFactory(UActorFactory* Factory)
{
	PlacementFactory = Factory;
	PlacementsChanged = true;

	if (AssetsToPlace.Num() == 1 && AssetsToPlace[0].IsValid())
	{
		AssetTypeToFactory.Add(*AssetsToPlace[0]->GetClass()->GetPathName(), PlacementFactory);
	}
}

UActorFactory* FNPCPaletteEdMode::FindLastUsedFactoryForAssetType(UObject* Asset) const
{
	if (Asset == NULL)
	{
		return NULL;
	}

	UActorFactory* LastUsedFactory = NULL;

	UClass* CurrentClass = Cast<UClass>(Asset);
	if (CurrentClass == NULL)
	{
		CurrentClass = Asset->GetClass();
	}

	while (LastUsedFactory == NULL && CurrentClass != NULL && CurrentClass != UClass::StaticClass())
	{
		const TWeakObjectPtr<UActorFactory>* FoundFactory = AssetTypeToFactory.Find(*CurrentClass->GetPathName());
		if (FoundFactory != NULL && FoundFactory->IsValid())
		{
			LastUsedFactory = FoundFactory->Get();
		}
		else
		{
			CurrentClass = CurrentClass->GetSuperClass();
		}
	}

	return LastUsedFactory;
}

void FNPCPaletteEdMode::ClearAssetsToPlace()
{
	AssetsToPlace.Empty();
	PlacedActors.Empty();
	PlacementsChanged = true;

	//m_CurrentEntity = nullptr;
}

void FNPCPaletteEdMode::SelectPlacedActors()
{
	const FScopedTransaction Transaction(NSLOCTEXT("BuilderMode", "SelectActors", "Select Actors"));

	const bool bNotifySelectNone = false;
	const bool bDeselectBSPSurfs = true;
	GEditor->SelectNone(bNotifySelectNone, bDeselectBSPSurfs);

	GEditor->GetSelectedActors()->BeginBatchSelectOperation();

	const bool bSelect = true;
	const bool bNotifyForActor = false;
	const bool bSelectEvenIfHidden = false;
	for (int Index = 0; Index < PlacedActors.Num(); Index++)
	{
		if (PlacedActors[Index].IsValid())
		{
			GEditor->GetSelectedActors()->Modify();
			GEditor->SelectActor(PlacedActors[Index].Get(), bSelect, bNotifyForActor, bSelectEvenIfHidden);
		}
	}

	GEditor->GetSelectedActors()->EndBatchSelectOperation();
	GEditor->NoteSelectionChange();
}

const TArray<TWeakObjectPtr<UObject>>& FNPCPaletteEdMode::GetCurrentlyPlacingObjects() const
{
	return AssetsToPlace;
}
