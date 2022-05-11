// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "FurnitureBase.h"
#include "MyJrpg/Interfaces/Buildable.h"
#include "MyJrpg/Widgets/World/Build/BuildWidgetCompo.h"
#include "Storage.generated.h"

class UInventory;
UCLASS()
class MYJRPG_API AStorage : public AFurnitureBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorage();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComp;
	UPROPERTY()
	UInventory* m_Inven;
	
protected:
	virtual void OnInteract() override;

	virtual void BeginPlay() override;
public:
	virtual void ConfirmBuild()override;
	
	virtual bool IsEraseable() override;
};
