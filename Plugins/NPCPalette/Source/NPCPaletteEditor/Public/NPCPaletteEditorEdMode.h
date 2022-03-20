// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "FNPCPlacementInfo.h"
#include "NPCPaletteDataAsset.h"

class ANPCPaletteGizmoActor;
struct FNPCPaletteItem;

class FNPCPaletteEdMode : public FEdMode
{

public:
	FNPCPaletteEdMode();
	
	virtual ~FNPCPaletteEdMode() override;

	virtual void Initialize() override;

	const static FEditorModeID EM_NPCPaletteEdModeId;

	// FEdMode interface
	virtual bool UsesToolkits() const override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient,float DeltaTime) override;
	virtual bool MouseEnter( FEditorViewportClient* ViewportClient,FViewport* Viewport,int32 x, int32 y ) override;
	virtual bool MouseLeave( FEditorViewportClient* ViewportClient,FViewport* Viewport ) override;
	virtual bool MouseMove( FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y ) override;
	virtual bool InputKey( FEditorViewportClient* InViewportClient, FViewport* InViewport, FKey InKey, EInputEvent InEvent ) override;
	virtual bool EndTracking( FEditorViewportClient* InViewportClient, FViewport* InViewport ) override;
	virtual bool StartTracking( FEditorViewportClient* InViewportClient, FViewport* InViewport ) override;

	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click ) override;
	virtual bool InputDelta( FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale ) override;
	virtual bool ShouldDrawWidget() const override;
	virtual bool UsesPropertyWidgets() const override;
	virtual bool IsCompatibleWith(FEditorModeID OtherModeID) const override;
	// End of FEdMode interface

public: //IPlacementMode
	
	
	void StopPlacing();

	bool IsCurrentlyPlacing() const { return AssetsToPlace.Num() > 0; };

	void StartPlacing(const TArray< FAssetData >& Assets, UActorFactory* Factory = NULL );

	 UActorFactory* GetPlacingFactory() const  { return PlacementFactory.Get(); }
	 void SetPlacingFactory( UActorFactory* Factory ) ;
	 UActorFactory* FindLastUsedFactoryForAssetType( UObject* Asset ) const ;

	 void AddValidFocusTargetForPlacement( const TWeakPtr< SWidget >& Widget )  { ValidFocusTargetsForPlacement.Add( Widget ); }
	 void RemoveValidFocusTargetForPlacement( const TWeakPtr< SWidget >& Widget )  { ValidFocusTargetsForPlacement.Remove( Widget ); }

	 const TArray< TWeakObjectPtr<UObject> >& GetCurrentlyPlacingObjects() const ;

private:
	

	void ClearAssetsToPlace();

	void SelectPlacedActors();

	bool AllowPreviewActors( FEditorViewportClient* ViewportClient ) const;

	void UpdatePreviewActors( FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y );

private:

	TWeakObjectPtr< UActorFactory > PlacementFactory;
	TArray< TWeakPtr< SWidget > > ValidFocusTargetsForPlacement;

	TArray< TWeakObjectPtr< UObject > > AssetsToPlace;
	TArray< TWeakObjectPtr< AActor > > PlacedActors;

	int32 ActiveTransactionIndex;

	bool PlacementsChanged;
	bool CreatedPreviewActors;
	bool PlacedActorsThisTrackingSession;
	bool AllowPreviewActorsWhileTracking;

	TMap< FName, TWeakObjectPtr< UActorFactory > > AssetTypeToFactory;

public:
	void SelectDropEntity(const FNPCPaletteItem& item);
		
	void OnSuccessActorPlaced(const FNPCPaletteItem& currentItem,AActor* actorPlaced);

	UTexture2D* GetAssetIcon(FEntityRow* row);

	FText GetAssetName(FEntityRow* row);
	
	void ClearPlacedAsset();

	void SpawnPlacedActor();


	void SetNPCAsset(UNPCPaletteDataAsset* assetNew);

	const UNPCPaletteDataAsset* GetNPCAsset() const;

	FDelegateHandle m_DeleHandle;

	void BindDelegate();

	void UnbindDelegate();

private:
	TArray<ANPCPaletteGizmoActor*> m_MyPlacedActors;

	const FNPCPaletteItem* m_DropNpcItem;
	
	TWeakObjectPtr<UNPCPaletteDataAsset> m_CurrentAsset;

	bool CreateNPCItemStruct(const FNPCSpawnData& dataIn,FNPCPaletteItem& outItem);
};
