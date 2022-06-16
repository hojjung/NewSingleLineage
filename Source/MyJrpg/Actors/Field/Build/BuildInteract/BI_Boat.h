// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "BI_Boat.generated.h"

/**
 * 물건너 맵에 갈 수 있게된다. 결국 맵의 리스트를 놓고 하면되나?
 * 배의 내구도가 해당 맵을 넘어갈때마다 조금씩 깎이고, 나무 재료를 계속 넣어줘야함
 */
UCLASS()
class MYJRPG_API UBI_Boat : public UBuildInteractBase
{
	GENERATED_BODY()
	
};
