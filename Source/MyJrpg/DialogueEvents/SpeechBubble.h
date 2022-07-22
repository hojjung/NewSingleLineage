// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "SpeechBubble.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USpeechBubble : public UDialogueEvents
{
	GENERATED_BODY()

protected:
	virtual void RecieveEventTriggered(APlayerController* ConsideringPlayer, AActor* NPCActor) override;
};


