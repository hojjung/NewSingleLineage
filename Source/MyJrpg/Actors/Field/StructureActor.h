#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructureActor.generated.h"

struct FBuildDataRow;
UCLASS()
class MYJRPG_API AStructureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AStructureActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_Mesh;
	UPROPERTY()
	TArray<UMaterialInterface*> m_AryMats;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void SetBuildData(const FBuildDataRow& data_row);
	
	void SetMat(UMaterialInterface* mat);

	void ConfirmBuild();
};
