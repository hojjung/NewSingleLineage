#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "MyJrpg/MyJrpg.h"
#include "MyAnimInstance.generated.h"

class AMyPlayerPawn;
class ACombatUnitPawn;
class UMyAnimInstance;

USTRUCT(BlueprintType)
struct FMyAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	
public:
	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;

	virtual void Update(float DeltaSeconds) override ;

	UPROPERTY(Transient)
	UMyAnimInstance* m_MyAnim = nullptr;
};

UCLASS()
class MYJRPG_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	FMyAnimInstanceProxy m_Proxy;
	UPROPERTY()
	ACombatUnitPawn* m_Owner;
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	bool m_bIsMoving;
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	bool m_bIsSneaking;
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	bool m_bIsLooting;
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	EStanceType m_Stance;	
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeInitializeAnimation() override;
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &m_Proxy;
	}
	
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	virtual void UpdateMoveFlag();

	UFUNCTION(BlueprintCallable, Category = "My Stance",meta = (BlueprintThreadSafe))
	bool IsStance(EStanceType t) const;
	UFUNCTION(BlueprintCallable, Category = "My Stance",meta = (BlueprintThreadSafe))
	bool IsNotStance(EStanceType t) const;
	UFUNCTION(BlueprintCallable, Category = "My Stance",meta = (BlueprintThreadSafe))
	int GetStanceIndex() const;
};