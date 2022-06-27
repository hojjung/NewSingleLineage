// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/MyPlayerController.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "UObject/NoExportTypes.h"
#include "MinimapManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMinimapManager : public UObject
{
	GENERATED_BODY()

protected:
	TArray<TWeakObjectPtr<AActor>> m_AryActors;

	TWeakObjectPtr<AMyPlayerController> m_PlCon;
	
public:
	void Init();
	
	void AddTrackActor(AActor* target);

public:
	bool IsVisible(AActor* target);
};


