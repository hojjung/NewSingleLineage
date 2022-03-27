// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "PreviewActor.generated.h"

UCLASS()
class MYJRPG_API APreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewActor();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshBody;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USceneCaptureComponent2D* m_Capture;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USpringArmComponent* m_Spring;
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* m_Light;
protected:
	UPROPERTY()
	UTextureRenderTarget2D* m_CaptureTexture;

	FDelegateHandle Handle1;

	FDelegateHandle Handle2;

	bool m_bTouched;

	FRotator m_InitVisualRot;
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void CalculateVisualActorRot(float delta);
	
public:
	void OnMeshVisualChanged(const FPlayerUnitEntityRow& charData);
	
	void ShowMeshWithTick();

	void HideMeshWithTick();
	
	void SetIsTouched(bool b);
	
	void RotatePawn(float delta_x);
};
