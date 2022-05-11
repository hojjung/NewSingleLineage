// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "FurnitureBase.h"
#include "PortalActor.generated.h"

UCLASS()
class MYJRPG_API APortalActor : public AFurnitureBase
{
	GENERATED_BODY()
	
public:	
	APortalActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComp;

protected:
	virtual void OnInteract() override;
};
