// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "InteractActorBase.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Interfaces/Buildable.h"
#include "MyJrpg/Widgets/World/Build/BuildWidgetCompo.h"
#include "Storage.generated.h"

UCLASS()
class MYJRPG_API AStorage : public AInteractActorBase, public IBuildable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorage();

protected:
	UPROPERTY(VisibleAnywhere)
	UBuildWidgetCompo* m_WidgetComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComp;
	UPROPERTY()
	UInventory* m_Inven;
	UPROPERTY()
	TArray<UMaterialInterface*> m_AryMats;
	const FBuildDataRow* m_Data;
protected:
	virtual void OnInteract() override;

	virtual void BeginPlay() override;
public:
	virtual const FBuildDataRow& GetBuildData() const override;
	virtual void SetBuildData(const FBuildDataRow& data)override;
	virtual void SetMat(UMaterialInterface* mat)override;
	virtual void ShowBuildWidget(bool b)override;
	virtual void ConfirmBuild()override;
	virtual void ShowSelect(bool b)override;
	virtual bool IsEraseable() override;
};
