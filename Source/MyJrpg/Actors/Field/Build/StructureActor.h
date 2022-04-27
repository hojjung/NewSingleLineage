#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Widgets/World/Build/BuildWidgetCompo.h"
#include "MyJrpg/Interfaces/Buildable.h"
#include "StructureActor.generated.h"

struct FBuildDataRow;
UCLASS()
class MYJRPG_API AStructureActor : public AStaticMeshActor , public IBuildable
{
	GENERATED_BODY()
	
public:	
	AStructureActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UBuildWidgetCompo* m_WidgetComp;
	UPROPERTY()
	TArray<UMaterialInterface*> m_AryMats;

	const FBuildDataRow* m_DataRow;
protected:
	virtual void BeginPlay() override;

public:
	virtual bool HasNextUpgrade() override;
	
	virtual void SetBuildData(const FBuildDataRow& data_row) override;
	
	virtual void SetMat(UMaterialInterface* mat) override;

	virtual void ConfirmBuild() override;

	virtual const FBuildDataRow& GetBuildData() const override;

	virtual void ShowBuildWidget(bool b) override;

	virtual void ShowSelect(bool b) override;

	virtual bool TryUpgrade();
	
};
