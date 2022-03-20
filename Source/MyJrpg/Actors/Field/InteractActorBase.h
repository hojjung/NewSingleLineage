// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Widgets/WidgetComponents/InteractWidget.h"
#include "InteractActorBase.generated.h"

//포탈?-플레이어는 던전에서 마을귀환밖에 못함
//창고
//상점
//퀘스트를 줄지도 모르는 NPC들
UCLASS()
class MYJRPG_API AInteractActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractActorBase();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character)
	UCapsuleComponent* m_Capsule;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UInteractWidget> m_ClassInteractWidget;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UWidgetComponent* m_InteractComp;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void OnInteract();

	float GetRadius() const;
};
