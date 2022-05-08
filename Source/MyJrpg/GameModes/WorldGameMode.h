// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWorldGameMode();

protected:
	virtual void Tick(float DeltaSeconds) override;
};
