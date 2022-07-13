// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "BI_Horse.generated.h"

/**
 * 무과금으로 걸어가는 속도가 빨라짐
 * 말에게 밥을 주면 내구도같은 체력이 생기고
 * 플레이어 에너지를 소비할때마다 줄어들음
 */
UCLASS()
class MYJRPG_API UBI_Horse : public UBuildInteractBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UInventory* m_Inven;
	UPROPERTY()
	AMyPlayerPawn* m_Player;
	
public:
	virtual void Init(const TArray<FString>& variable, UInventory* inven) override;
	
	virtual UInventory* GetItemHolder() override;
	
	virtual bool IsEraseable() override;

	virtual void OnInteract() override;
};
