// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveIndicator.generated.h"

UCLASS()
class MYJRPG_API AMoveIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMoveIndicator();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_Decal;

};
