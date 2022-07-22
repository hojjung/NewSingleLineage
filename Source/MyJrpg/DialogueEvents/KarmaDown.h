// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "KarmaDown.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UKarmaDown : public UDialogueEvents
{
	GENERATED_BODY()

protected:
	virtual void RecieveEventTriggered(APlayerController* ConsideringPlayer, AActor* NPCActor) override;
};
