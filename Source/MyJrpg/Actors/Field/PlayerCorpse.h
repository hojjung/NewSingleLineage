// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Items/Inventory.h"
#include "PlayerCorpse.generated.h"

//만들고보니 그냥 스트럭처로 만들고 플레이어 죽은 인벤토리는 다른매니저가 관리하는게 나음
UCLASS()
class MYJRPG_API APlayerCorpse : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerCorpse();

protected:
	UPROPERTY()
	UInventory* m_Inven;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetCorpse(AMy);
};
