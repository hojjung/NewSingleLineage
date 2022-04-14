// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyMovement.generated.h"

class INavigationDataInterface;
/**
 * 
 */
UCLASS()
class MYJRPG_API UMyMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()

protected:
	FVector m_ImpactVector;
	
public:
	UMyMovement(const FObjectInitializer& obj);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetImpact(FVector v);

	virtual void SetActive(bool bNewActive, bool bReset=false) override;
	
protected:
	void TickRotate(float deltaTime);

	FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice=0.f, const FVector& MoveDelta = FVector::ZeroVector) override;
};


