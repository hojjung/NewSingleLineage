#pragma once

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Animations/MyAnimInstance.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseUnitPawn.generated.h"

class UMyMovement;
class UWidgetSpeechBubbleComp;
class UWidgetPawnInfoComp;
class UInventory;
class UUnitEntityAsset;
struct FNpcUnitEntityRow;
UCLASS()
class MYJRPG_API ABaseUnitPawn : public APawn	
{
	GENERATED_BODY()
	
public:
	ABaseUnitPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character)
	UCapsuleComponent* m_Capsule;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character,meta = ( AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "AI")
	UPathFollowingComponent* m_PFComp;//UCrowdFollowingComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Movement")
	UMyMovement* m_Movement;
	
	TWeakObjectPtr<UUnitEntityAsset> m_EntityAsset;

	FTimerHandle m_MoveStopTimer;
public:
	UPROPERTY()
	FText m_PawnName;

protected:
	virtual void LoadSetSkMeshAnim(UUnitEntityAsset* asset);

	void ActiveMovement();
	//PathFollow	
	FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

	void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query,FNavPathSharedPtr& OutPath) const;

	bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

	FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

public:
	virtual void SetEntity(const FNpcUnitEntityRow& unitEntityRow);//Init
	
	virtual EPathFollowingRequestResult::Type MoveToLocation(FVector loc, float acceptRadius = 0.f);

	virtual EPathFollowingRequestResult::Type MoveToActor(AActor* target, float acceptRadius = 0);

	bool LineOfSightTo(const AActor* Other) const;

public://Anim
	float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	float PlayAnim(UAnimSequenceBase* animAsset,bool isLoop=false);

	void StopAnimMontage();

	UAnimMontage* GetCurrentMontage();

public:
	UCapsuleComponent* GetCapsule() const;

	USkeletalMeshComponent* GetSkMesh() const;

	UPathFollowingComponent* GetPfComp() const;

	UMyMovement* GetMove() const;

	FText GetPawnName() const;

	virtual bool IsMoving() const;

	void ClearStopMoveDelegate();
public:
	void SetActive(bool isActive);

	float GetRadius() const;
	
	void StopMove();
	
	virtual bool IsRange();
};

