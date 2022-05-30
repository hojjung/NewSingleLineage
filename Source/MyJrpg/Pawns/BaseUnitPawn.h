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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character)
	USkeletalMeshComponent* m_BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "AI")
	UPathFollowingComponent* m_PFComp;//UCrowdFollowingComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Movement")
	UMyMovement* m_Movement;
	
	TSoftObjectPtr<UUnitEntityAsset> m_EntityAsset;

	FTimerHandle m_MoveStopTimer;
	
	FText m_PawnName;

	FName m_EntityID;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void LoadSetSkMeshAnim(TSoftObjectPtr<UUnitEntityAsset> asset);

	void ActiveMovement();
	//PathFollow	
	FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

	void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query,FNavPathSharedPtr& OutPath) const;

	bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

	FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

	USkeletalMeshComponent* CreateSkMeshComp(FName keyID);
public:
	virtual void SetEntity(const FName& id, const FNpcUnitEntityRow& unitEntityRow);//Init
	
	virtual FPathFollowingRequestResult MoveToLocation(FVector loc, float acceptRadius = 0.f);

	virtual FPathFollowingRequestResult MoveToActor(AActor* target, float acceptRadius = 0);

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
	float GetRadius() const;
	
	void StopMove();
	
	virtual bool IsRange();

	const FName& GetEntityID() const;

	void SetActorFeetLocation(FVector loc);
};

