// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "InteractActorBase.h"
#include "GameFramework/Actor.h"
#include "Storage.generated.h"

UCLASS()
class MYJRPG_API AStorage : public AInteractActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorage();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComp;

protected:
	virtual void OnInteract() override;
};
