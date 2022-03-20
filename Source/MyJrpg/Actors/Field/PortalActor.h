// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "InteractActorBase.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"

UCLASS()
class MYJRPG_API APortalActor : public AInteractActorBase
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
