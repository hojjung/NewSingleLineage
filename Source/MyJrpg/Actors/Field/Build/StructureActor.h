#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Widgets/World/Build/BuildWidgetCompo.h"
#include "StructureActor.generated.h"

struct FBuildDataRow;
UCLASS()
class MYJRPG_API AStructureActor : public AStaticMeshActor
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
	virtual void SetBuildData(const FBuildDataRow& data_row);
	
	virtual void SetMat(UMaterialInterface* mat);

	virtual void ConfirmBuild();

	const FBuildDataRow& GetBuildData();

	void ShowBuildWidget(bool b);
};
