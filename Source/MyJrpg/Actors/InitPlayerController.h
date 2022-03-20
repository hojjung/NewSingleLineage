// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AInitPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AInitPlayerController();

	virtual void SetupInputComponent() override;
	
	void ExitGame();

	virtual void BeginPlay() override;
};
