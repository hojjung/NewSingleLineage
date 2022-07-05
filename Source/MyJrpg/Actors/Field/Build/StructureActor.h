#pragma once

#include "CoreMinimal.h"
#include "BuildInteract/BuildInteractBase.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Widgets/World/Build/BuildWidgetCompo.h"
#include "StructureActor.generated.h"

class UInventory;
struct FBuildDataRow;
UCLASS()
class MYJRPG_API AStructureActor : public AActor, public IFocusable, public INavAgentInterface
{
	GENERATED_BODY()
public:
	AStructureActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UMinimapIconComp* m_IconMeshComp;
	UPROPERTY(VisibleAnywhere)
	UBuildWidgetCompo* m_WidgetComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComp;
	UPROPERTY()
	UBuildInteractBase* m_BuildInteract;
	UPROPERTY()
	TArray<UMeshComponent*> m_AryMeshCompos;
	UPROPERTY()
	TArray<UMeshComponent*> m_AryMeshComposColl;
	TArray<TArray<TStrongObjectPtr<UMaterialInterface>>> m_AryAryMats;
	
	const FBuildDataRow* m_BuildData;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnArrived();

public:
	virtual const FBuildDataRow& GetBuildData() const ;
	virtual void SetBuildData(const FBuildDataRow& data);
	virtual void SetMat(UMaterialInterface* mat);
	virtual void ShowBuildWidget(bool b);
	virtual void ConfirmBuild(UInventory* inven = nullptr);
	virtual void ShowSelect(bool b);
	virtual bool IsEraseable() ;
	virtual void SetColl(bool b) ;
	
	bool IsUpgradeable();

	virtual void OnInteract() override;

	virtual FVector GetNavAgentLocation() const override;

	virtual void GetMoveGoalReachTest(const AActor* MovingActor, const FVector& MoveOffset, FVector& GoalOffset,float& GoalRadius, float& GoalHalfHeight) const override;

	UInventory* GetItemHolder();

	virtual UMinimapIconComp* GetIconMeshComp() override;

	virtual bool IsFocusable() override;
};
