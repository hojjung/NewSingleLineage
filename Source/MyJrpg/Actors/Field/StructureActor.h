#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "StructureActor.generated.h"

struct FBuildDataRow;
UCLASS()
class MYJRPG_API AStructureActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	AStructureActor();

protected:
	UPROPERTY()
	TArray<UMaterialInterface*> m_AryMats;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetBuildData(const FBuildDataRow& data_row);
	
	virtual void SetMat(UMaterialInterface* mat);

	virtual void ConfirmBuild();
};
