// All right Reserve 2021 HereticByte

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "CoreMinimal.h"
#include "AITypes.h"
#include "MoveIndicator.h"
#include "GameFramework/PlayerController.h"
#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidgetComp.h"
#include "Navigation/PathFollowingComponent.h"

#include "MyPlayerController.generated.h"


class ABaseUnitPawn;
class AMyAllyPawn;
UCLASS()
class MYJRPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<UInGameTextWidgetComp*> m_AryIGWC;
	UPROPERTY()
	TArray<AActor*> m_AryIgnoreActors;
	UPROPERTY()
	int m_CompUseIndex;
	
protected:
	virtual void BeginPlay() override;

	void InitWidget();

	void CreateIGWC(int count);

	UInGameTextWidgetComp* GetIGWC(); 

	virtual void SetupInputComponent() override;

	void ExitGame();

	bool CheckInteract();
	
public:
	void ShowInGameWorldText(float number,ABaseUnitPawn* interactActor,ETextType dmgPopup); //target
	
	void ShowInGameWorldText(const FString& stringWant, const ABaseUnitPawn* interactActor, ETextType dmgPopup); //target

	UFUNCTION()
	void BackToSelectMenu();
	UFUNCTION()
	void OnTouchPressed();
};
