// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyMovement.generated.h"

class ACombatUnitPawn;
class INavigationDataInterface;
/**
 * 
 */
UCLASS()
class MYJRPG_API UMyMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	UMyMovement(const FObjectInitializer& obj);

protected:
	UPROPERTY()
	ACombatUnitPawn* m_Owner;
	
public:
	float m_fSpeedMultiple;
	
protected:
	FVector m_ImpactVector;
	
	void SnapToNav();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetImpact(FVector v);

	virtual void SetActive(bool bNewActive, bool bReset=false) override;
	
protected:
	virtual void BeginPlay() override;
	
	void TickRotate(float deltaTime);

	FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice=0.f, const FVector& MoveDelta = FVector::ZeroVector) override;

	bool CanStepUp(const FHitResult& Hit) const;

	virtual  void TrySnapFloor();
};


