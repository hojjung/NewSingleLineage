#include "BaseUnitPawn.h"
#include "DetourCrowdAIController.h"
#include "NavigationSystem.h"
#include "NetworkingDistanceConstants.h"
#include "Animation/AnimInstance.h"
#include "Components/MyMovement.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "Navigation/CrowdFollowingComponent.h"

ABaseUnitPawn::ABaseUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule00"));
	m_Capsule->InitCapsuleSize(34.0f, 88.0f);
	m_Capsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	m_Capsule->CanCharacterStepUpOn = ECB_No;
	m_Capsule->SetShouldUpdatePhysicsVolume(false);
	m_Capsule->SetCanEverAffectNavigation(false);
	m_Capsule->bDynamicObstacle = false;
	m_Capsule->AreaClass = nullptr;
	RootComponent = m_Capsule;
	m_Capsule->bReceivesDecals = false;
	//
	m_BodyMesh = CreateSkMeshComp(TEXT("BodyMesh"));
	//
	m_Movement = CreateDefaultSubobject<UMyMovement>(TEXT("m_Movement"));
	m_Movement->UpdatedComponent = m_Capsule;
	m_Movement->MaxSpeed=250.f;
	
	AIControllerClass = nullptr;
	
	m_PFComp = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));
}

USkeletalMeshComponent* ABaseUnitPawn::CreateSkMeshComp(FName keyID)
{
	USkeletalMeshComponent* skMesh = CreateDefaultSubobject<USkeletalMeshComponent>(keyID);
	skMesh->SetupAttachment(RootComponent);
	skMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
	skMesh->SetGenerateOverlapEvents(false);
	skMesh->SetCanEverAffectNavigation(false);
	skMesh->SetRelativeLocation(FVector(0, 0, -88));
	skMesh->SetRelativeRotation(FRotator(0, -90.f, 0.f));
	skMesh->bReceivesDecals = false;
	skMesh->bOwnerNoSee = false;
	//
	skMesh->bCastDynamicShadow = false; //chanage for mobile
	skMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;//최적화
	skMesh->bAffectDynamicIndirectLighting = true;
	skMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	skMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	skMesh->bEnableUpdateRateOptimizations = true;
	skMesh->bComponentUseFixedSkelBounds = true;
	return skMesh;
}

void ABaseUnitPawn::SetEntity(const FName& id, const FNpcUnitEntityRow& unitEntityRow)
{
	m_EntityID = id;
	
	m_PFComp->SetMovementComponent(m_Movement);
	
	m_PFComp->Initialize();
	
	m_Movement->SetPathFollowingAgent(m_PFComp);
	
	LoadSetSkMeshAnim(unitEntityRow.m_UnitDataAsset);

	m_BodyMesh->SetRelativeScale3D(FVector(unitEntityRow.m_fScale));

	m_PawnName = unitEntityRow.m_ShowingName;

	m_Capsule->SetCapsuleRadius(unitEntityRow.m_fCapsuleRadius);

	m_Movement->NavAgentProps.AgentRadius = unitEntityRow.m_fCapsuleRadius;
}

void ABaseUnitPawn::LoadSetSkMeshAnim(TSoftObjectPtr<UUnitEntityAsset> asset)
{
	m_EntityAsset =  UMyAssetManager::Get()->LoadUnitAsset(asset);

	m_BodyMesh->SetSkeletalMesh(m_EntityAsset->GetSkMesh());

	m_BodyMesh->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);

	m_BodyMesh->SetAnimClass(m_EntityAsset->m_AnimBP.Get());

	m_BodyMesh->AddRelativeRotation(FRotator(0,asset->m_RotYawOffset,0));
}

void ABaseUnitPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_EntityAsset.Reset();
}

void ABaseUnitPawn::ActiveMovement()
{
	m_Movement->SetActive(true);
}

FPathFollowingRequestResult ABaseUnitPawn::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath)
{
	FPathFollowingRequestResult ResultData;
	ResultData.Code = EPathFollowingRequestResult::Failed;

	if (MoveRequest.IsValid() == false)
	{
		PRINTF("InvalidRequest");
		return ResultData;
	}
	
	bool bCanRequestMove = true;
	
	bool bAlreadyAtGoal = false;

	if (!MoveRequest.IsMoveToActorRequest())
	{
		if (MoveRequest.GetGoalLocation().ContainsNaN() || FAISystem::IsValidLocation(MoveRequest.GetGoalLocation()) ==
			false)
		{
			bCanRequestMove = false;
		}

		if (bCanRequestMove && MoveRequest.IsProjectingGoal())
		{
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			const FNavAgentProperties& AgentProps = GetNavAgentPropertiesRef();
			FNavLocation ProjectedLocation;

			if (NavSys && !NavSys->ProjectPointToNavigation(MoveRequest.GetGoalLocation(), ProjectedLocation,
			                                                INVALID_NAVEXTENT, &AgentProps))
			{
				bCanRequestMove = false;
			}

			MoveRequest.UpdateGoalLocation(ProjectedLocation.Location);
		}

		bAlreadyAtGoal = bCanRequestMove && m_PFComp->HasReached(MoveRequest);
	}
	else
	{
		bAlreadyAtGoal = bCanRequestMove && m_PFComp->HasReached(MoveRequest);
	}

	if (bAlreadyAtGoal)
	{
		ResultData.MoveId = m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
		ResultData.Code = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else if (bCanRequestMove)
	{
		FPathFindingQuery PFQuery;

		const bool bValidQuery = BuildPathfindingQuery(MoveRequest, PFQuery);
		if (bValidQuery)
		{
			FNavPathSharedPtr Path;
			FindPathForMoveRequest(MoveRequest, PFQuery, Path);

			const FAIRequestID RequestID = Path.IsValid()
				                               ? RequestMove(MoveRequest, Path)
				                               : FAIRequestID::InvalidRequest;
			if (RequestID.IsValid())
			{
				ResultData.MoveId = RequestID;
				ResultData.Code = EPathFollowingRequestResult::RequestSuccessful;

				if (OutPath)
				{
					*OutPath = Path;
				}
			}
		}
	}

	if (ResultData.Code == EPathFollowingRequestResult::Failed)
	{
		ResultData.MoveId = m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
	}

	return ResultData;
}

void ABaseUnitPawn::FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query,
                                           FNavPathSharedPtr& OutPath) const
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		FPathFindingResult PathResult = NavSys->FindPathSync(Query);
		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				if (MoveRequest.IsMoveToActorRequest())
				{
					PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
				}

				PathResult.Path->EnableRecalculationOnInvalidation(true);
				OutPath = PathResult.Path;
			}
		}
	}
}

bool ABaseUnitPawn::BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const
{
	bool bResult = false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const ANavigationData* NavData = (NavSys == nullptr)
		                                 ? nullptr
		                                 : MoveRequest.IsUsingPathfinding()
		                                 ? NavSys->GetNavDataForProps(GetNavAgentPropertiesRef())
		                                 : NavSys->GetAbstractNavData();

	if (NavData)
	{
		FVector GoalLocation = MoveRequest.GetGoalLocation();
		if (MoveRequest.IsMoveToActorRequest())
		{
			const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
			if (NavGoal)
			{
				const FVector Offset = NavGoal->GetMoveGoalOffset(this);
				GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(),
				                                              NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
			}
			else
			{
				GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
			}
		}

		FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(
			*NavData, this, MoveRequest.GetNavigationFilter());
		Query = FPathFindingQuery(*this, *NavData, GetNavAgentLocation(), GoalLocation, NavFilter);
		Query.SetAllowPartialPaths(MoveRequest.IsUsingPartialPaths());

		if (m_PFComp)
		{
			m_PFComp->OnPathfindingQuery(Query);
		}

		bResult = true;
	}

	return bResult;
}

FAIRequestID ABaseUnitPawn::RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path)
{
	uint32 RequestID = FAIRequestID::InvalidRequest;
	RequestID = m_PFComp->RequestMove(MoveRequest, Path);
	return RequestID;
}

float ABaseUnitPawn::GetRadius() const
{
	return m_Capsule->GetScaledCapsuleRadius();
}

void ABaseUnitPawn::StopMove()
{
	m_Movement->StopMovementImmediately();
	m_PFComp->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
}

bool ABaseUnitPawn::IsRange()
{
	return false;
}

const FName& ABaseUnitPawn::GetEntityID() const
{
	return m_EntityID;
}

void ABaseUnitPawn::SetActorFeetLocation(FVector loc)
{
	FVector NewLoc = loc;

	NewLoc.Z += GetCapsule()->Bounds.BoxExtent.Z;
		
	SetActorLocation(NewLoc);
}

FPathFollowingRequestResult ABaseUnitPawn::MoveToLocation(FVector loc, float acceptRadius)
{
	if (m_PFComp && m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFComp->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}

	FAIMoveRequest MoveReq(loc);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);
	MoveReq.SetProjectGoalLocation(true);
	MoveReq.SetNavigationFilter(UNavigationQueryFilter::StaticClass());
	MoveReq.SetAcceptanceRadius(acceptRadius);
	MoveReq.SetReachTestIncludesAgentRadius(true);
	MoveReq.SetCanStrafe(true);

	return MoveTo(MoveReq);
}

FPathFollowingRequestResult ABaseUnitPawn::MoveToActor(AActor* target, float acceptRadius) //바디메쉬크기가 260으로 나온다
{
	if (m_PFComp && m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFComp->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}

	FAIMoveRequest MoveReq(Cast<AActor>(target));
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);
	MoveReq.SetNavigationFilter(UNavigationQueryFilter::StaticClass());
	MoveReq.SetAcceptanceRadius(acceptRadius);
	MoveReq.SetReachTestIncludesAgentRadius(false);
	MoveReq.SetReachTestIncludesGoalRadius(false);
	MoveReq.SetCanStrafe(true);

	return MoveTo(MoveReq);
}

bool ABaseUnitPawn::LineOfSightTo(const AActor* Other) const
{
	if (!Other)
	{
		return false;
	}

	FVector ViewPoint = GetActorLocation();

	FCollisionQueryParams CollisionParms(SCENE_QUERY_STAT(LineOfSight), false, Other);
	
	CollisionParms.AddIgnoredActor(this);
	
	FVector TargetLocation = Other->GetActorLocation();
	
	bool bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, TargetLocation, ECC_Visibility, CollisionParms);
	
	return !bHit;
}
//////
float ABaseUnitPawn::PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate, FName StartSectionName)
{
	auto* AnimInstance = m_BodyMesh->GetAnimInstance();

	//AnimInstance->RootMotionMode
	if (anim_montage && AnimInstance)
	{
		float Duration = AnimInstance->Montage_Play(anim_montage, InPlayRate);

		if (Duration > 0.f)
		{
			FName SectioNName;

			if (StartSectionName != NAME_None) //섹션지정시
			{
				SectioNName = StartSectionName;
			}
			else
			{
				SectioNName = anim_montage->GetSectionName(0);
			}
			
			AnimInstance->Montage_JumpToSection(SectioNName, anim_montage);			

			int Index = anim_montage->GetSectionIndex(SectioNName);

			Duration = anim_montage->GetSectionLength(Index);

			Duration = (Duration / (InPlayRate * anim_montage->RateScale)); //가속된만큼 빠르게

			ClearStopMoveDelegate();
			m_Movement->SetActive(false);
			GetWorldTimerManager().SetTimer(m_MoveStopTimer, this, &ABaseUnitPawn::ActiveMovement, Duration, false);
			
			return Duration;
		}
	}

	return 0.f;
}

float ABaseUnitPawn::PlayAnim(UAnimSequenceBase* animAsset, bool isLoop)
{
	m_BodyMesh->PlayAnimation(animAsset, isLoop);

	return animAsset->SequenceLength;
}

void ABaseUnitPawn::StopAnimMontage()
{
	UAnimMontage* MontageToStop = GetCurrentMontage();

	if (!MontageToStop)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = m_BodyMesh->GetAnimInstance();

	bool bShouldStopMontage = AnimInstance && MontageToStop && !AnimInstance->Montage_GetIsStopped(MontageToStop);

	if (bShouldStopMontage)
	{
		AnimInstance->Montage_Stop(MontageToStop->BlendOut.GetBlendTime(), MontageToStop);
	}
}

UAnimMontage* ABaseUnitPawn::GetCurrentMontage()
{
	UAnimInstance* AnimInstance = m_BodyMesh->GetAnimInstance();

	if (AnimInstance)
	{
		return AnimInstance->GetCurrentActiveMontage();
	}

	return nullptr;
}

UCapsuleComponent* ABaseUnitPawn::GetCapsule() const
{
	return m_Capsule;
}

USkeletalMeshComponent* ABaseUnitPawn::GetSkMesh() const
{
	return m_BodyMesh;
}

UPathFollowingComponent* ABaseUnitPawn::GetPfComp() const
{
	return m_PFComp;
}

UMyMovement* ABaseUnitPawn::GetMove() const
{
	return Cast<UMyMovement>(GetMovementComponent());
}

bool ABaseUnitPawn::IsMoving() const
{
	return !GetMovementComponent()->Velocity.IsZero();
}

void ABaseUnitPawn::ClearStopMoveDelegate()
{
	GetWorldTimerManager().ClearTimer(m_MoveStopTimer);
}

FText ABaseUnitPawn::GetPawnName() const
{
	return m_PawnName;
}

FVector ABaseUnitPawn::GetNavAgentLocation() const
{
	return GetActorLocation() - FVector(0.f, 0.f, GetCapsule()->Bounds.BoxExtent.Z);
}