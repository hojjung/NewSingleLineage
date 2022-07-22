#include "MyPlayerPawn.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Logics/PlSkillAuto.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Animations/MyAnimInstance.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"
#include "MyJrpg/Pawns/Components/MyMovement.h"
#include "MyJrpg/Pawns/Logics/AI/AI_Logic/Logic_Player.h"
#include "MyJrpg/Pawns/Logics/AI/AI_Sensor/Sensor_Player.h"
#include "MyJrpg/Widgets/WidgetComponents/InGameTextWidget.h"

// Sets default values
AMyPlayerPawn::AMyPlayerPawn(const FObjectInitializer& objInit):Super(objInit)
{
	m_Movement->MaxSpeed=FGlobalVariable::HERO_DEFAULT_SPEED;

	m_bIsSneakAttack = false;
	m_bIsSkillUsing = false;
	m_bCanMoveInSkill = false;
	m_bIsInvincible = false;
	m_bIsSneaking = false;
	m_bUseFsmTick = false;	

	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);
	EndBuildCam();
	m_DissolveCam->m_SocketOffset = FVector(0,0,-30);
	m_DissolveCam->CameraLagSpeed=30;
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_TopCamera"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 60.f;
	//
	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	m_bOnceMoveLock = false;
	//
	m_ShadowMeshComp->SetRelativeScale3D(FVector(4));

	m_bIsInteracting = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D>
	FoundIcon(TEXT("Texture2D'/Game/Sprites/UI/Arrow_Pl.Arrow_Pl'"));
	m_MinimapIcon = FoundIcon.Object;

	SetAttackRange(250);

	m_IconComp->SetLayerHeight(500.f);
}

void AMyPlayerPawn::CreateFocusActor()
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	m_FocusIndicator = GetWorld()->SpawnActor<AMoveIndicator>(AMoveIndicator::StaticClass(),FVector(0,0,0),FRotator(0,0,0),Param);
	m_FocusIndicator->SetActorHiddenInGame(true);
}

void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_PFComp->OnRequestFinished.AddUObject(this, &AMyPlayerPawn::OnRequestMoveDone);

	m_AryIgnores.Reset();
	
	m_AryIgnores.Add(this);

	m_DissolveCam->SetActive(true);

	SetPlayerEntity();

	CreateFocusActor();
}

void AMyPlayerPawn::SetPlayerEntity()
{
	const TSoftObjectPtr<UHumanAsset>& HumanAsset = UMyGameInstance::Get->m_PlayerStatManager->GetUnitAsset();
	
	LoadSetSkMeshAnim(HumanAsset);
	
	ULogic_Player* Player = NewObject<ULogic_Player>(this,ULogic_Player::StaticClass());

	m_AiFsm = Player;

	m_AiFsm->Init(this);
	
	m_AiSensor = NewObject<USensor_Player>(this,USensor_Player::StaticClass());
	
	m_AiSensor->Init(this);
	
	SetIcon();
}

void AMyPlayerPawn::MoveForward(float AxisValue)
{
	m_Input.X = AxisValue;

	if (GetController() && (AxisValue != 0.0f))
	{
		const FRotator Rotation = m_TopCamera->GetComponentRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		m_DeltaX = Direction*AxisValue;
	}
}

void AMyPlayerPawn::MoveRight(float AxisValue)
{
	m_Input.Y = AxisValue;

	if (GetController() && (AxisValue != 0.0f))
	{
		const FRotator Rotation = m_TopCamera->GetComponentRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		m_DeltaY = Direction*AxisValue;
	}
}

void AMyPlayerPawn::ShowPopupText(float nbr, ETextType t)
{
	ETextType Type;
	
	if(t == ETextType::NormalRight)
	{
		Type = ETextType::PlDmg;
	}
	else
	{
		Type = t;
	}
	Super::ShowPopupText(nbr, Type);
}

void AMyPlayerPawn::CancelInteract()
{
	if(!GetInteracting())
	{
		return;
	}
	UMyLib::GetCanvas()->GetWaitInteract()->HideInteract();
		
	GetWorldTimerManager().ClearTimer(m_WaitInteractTimer);
		
	SetInteracting(false);

	m_OnCancelInteract.ExecuteIfBound();
	
	m_OnCancelInteract.Unbind();
}

void AMyPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(m_bIsSkillUsing && !m_bCanMoveInSkill)
	{
		return;
	}
	if (IsInputMoving() && IsAlive())//!m_bIsSkillUsing
	{
		if(!m_bCanMoveInSkill)
		{
			StopAnimMontage();
		}
		
		CancelInteract();
		
		ClearStopMoveDelegate();
		
		m_Movement->SetActive(true);


		FVector Loc = GetCapsule()->GetComponentLocation();

		FVector Delta = (m_DeltaX + m_DeltaY) * 100.0f;

		FVector Dest = Loc+Delta;
		
		FHitResult Hit;
	
		if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), Loc, Loc + Delta,ETraceTypeQuery::TraceTypeQuery3,
			false,m_AryIgnores, EDrawDebugTrace::None,Hit,true))
		{
			Dest = Hit.Location;
		}

		MoveToLocation(Dest,0);

		m_DeltaX = FVector::ZeroVector;
		
		m_DeltaY = FVector::ZeroVector;
	}
}

// Called to bind functionality to input
void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerPawn::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Sneak"), EInputEvent::IE_Pressed,this, &AMyPlayerPawn::SetSneak);
}

bool AMyPlayerPawn::IsInputMoving()
{
	return !m_Input.IsZero();
}

void AMyPlayerPawn::SetPlayerSkMeshDefault()
{
	LoadSetSkMeshAnim(m_EntityAsset);
}

void AMyPlayerPawn::RequestAttack()
{
	if (m_bIsSkillUsing || IsMoving())
	{
		return;
	}
	ACombatUnitPawn* FocusActor = GetFocusedTarget<ACombatUnitPawn>();
	if(!FocusActor)
	{
		TryAttack_External();
		return;
	}
	RequestInteract(FocusActor, FVoidVoid::CreateUObject(this, &AMyPlayerPawn::TryAttack_External),GetAttackRange());
}

void AMyPlayerPawn::TryAttack_External()
{
	float Len = TryAttack();

	if(Len > 0.f && IsSneak())
	{
		AMonsterPawn* Target = GetFocusedTarget<AMonsterPawn>();
		
		if(Target && Target->GetFocusedTarget<>() != this)
		{
			m_bIsSneakAttack = true;
		}
	}
	SetInteracting(false);
}

void AMyPlayerPawn::SubDmgFromHp(float dmg)
{
	UMyGameInstance::Get->m_PlayerStatManager->SubDmgFromHp(dmg);
}

const FStatGroup& AMyPlayerPawn::GetStat() const
{
	return UMyGameInstance::Get->m_PlayerStatManager->GetStat();
}

bool AMyPlayerPawn::IsManualMoving()
{
	return IsInputMoving();// || m_PFComp->GetStatus()==EPathFollowingStatus::Moving
}

void AMyPlayerPawn::SetAutoCombat(bool useAuto)
{
	SetEnableFsm(useAuto);

	if(!useAuto)
	{
		StopMove();
		CancelInteract();
	}
	SetFocusedTarget(nullptr);
}

void AMyPlayerPawn::ShowIndicator(IFocusable* target)
{
	if(!target)
	{
		m_FocusIndicator->SetActorHiddenInGame(true);
		return;
	}
	if(GetFocusedTarget<>() == target)
	{
		return;
	}
	m_FocusIndicator->SetActorHiddenInGame(false);

	INavAgentInterface* FocusActor = Cast<INavAgentInterface>(target);

	FVector Loc = FocusActor->GetNavAgentLocation();

	FAttachmentTransformRules Rule(EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,false);

	AActor* TargetActor = Cast<AActor>(target);
	m_FocusIndicator->AttachToActor(TargetActor, Rule);
	m_FocusIndicator->SetActorLocation(Loc);
	m_FocusIndicator->ResizeBound(FocusActor);
}

void AMyPlayerPawn::SetFocusedTarget(IFocusable* target)
{
	if(m_bIsInteracting)
	{
		return;
	}
	ShowIndicator(target);
	
	Super::SetFocusedTarget(target);

	m_OnFocus.Broadcast(Cast<IFocusable>(m_FocusedTarget.GetObject()));
	
	ShowIndicator(target);
}

void AMyPlayerPawn::PlayTookHitMontage()
{
	//not use
}

bool AMyPlayerPawn::IsAlive()
{
	float Hp = UMyGameInstance::Get->m_PlayerStatManager->GetStat().m_Hp;
	
	return Hp > 0.f;
}

void AMyPlayerPawn::SetSkillUsing(bool b)
{
	m_bIsSkillUsing = b;
}

void AMyPlayerPawn::SetCanMoveInSkill(bool b)
{
	m_bCanMoveInSkill = b;
}

bool AMyPlayerPawn::GetSkillUsing()
{
	return m_bIsSkillUsing;
}

bool AMyPlayerPawn::CanMoveInSkill()
{
	return m_bCanMoveInSkill;
}

void AMyPlayerPawn::UpdateStat(const FStatGroup& stat_group)
{
	m_Movement->MaxSpeed = stat_group.m_MoveSpeed;
}

void AMyPlayerPawn::DealBaseMeleeAttack()
{
	if(!GetFocusedTarget())
	{
		return ;
	}

	ACombatUnitPawn* Pawn =  Cast<ACombatUnitPawn>(GetFocusedTarget());

	if(!Pawn || !Pawn->IsAlive())
	{
		return ;
	}
	float Dmg = GetStat().m_Dmg;

	if(m_bIsSneakAttack)
	{
		Dmg *= 1.5f;
		m_bIsSneakAttack = false;
		SetSneak();
	}
	Pawn->TakeDmg(Dmg,this);

	

	UMyGameInstance::Get->m_EquipManager->ReduceDurability(EEquipSlotType::Weapon,1);
}

void AMyPlayerPawn::ShootBaseRangeAttack()
{
	m_Pool->ShootBullet(TEXT("LeftHandSocket"),GetFocusedTarget<ACombatUnitPawn>());
	
	UMyGameInstance::Get->m_EquipManager->ReduceDurability(EEquipSlotType::Weapon,1);
}

void AMyPlayerPawn::SetSneak()
{
	m_bIsSneaking = !m_bIsSneaking;

	if(m_bIsSneaking)
	{
		m_Movement->m_fSpeedMultiple = 0.68f;
	}
	else
	{
		m_Movement->m_fSpeedMultiple = 1.f;
	}
}

void AMyPlayerPawn::SetInteracting(bool b)
{
	m_bIsInteracting = b;
}

bool AMyPlayerPawn::GetInteracting() const
{
	return m_bIsInteracting;
}

void AMyPlayerPawn::WaitInteract(float interactTime, const FVoidVoid& delegate)
{
	SetInteracting(true);
	GetWorldTimerManager().SetTimer(m_WaitInteractTimer,delegate,interactTime,false);
	UMyLib::GetCanvas()->GetWaitInteract()->ShowInteract(interactTime);
}

void AMyPlayerPawn::RequestInteract(AActor* target, const FVoidVoid& delegate, float r)
{
	FPathFollowingRequestResult Result = MoveToActor(target, r);

	if(Result.Code == EPathFollowingRequestResult::Type::Failed)
	{
		PRINTF("EPathFollowingRequestResult::Type::Failed");
		return;
	}
	if(Result.Code == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_ReqID = FAIRequestID(); 
		HomingRotateToTarget(0);
		delegate.ExecuteIfBound();
		return;
	}
	
	m_ReqID = Result.MoveId;

	m_OnRequestDone = delegate;
}

void AMyPlayerPawn::OnRequestMoveDone(FAIRequestID id, const FPathFollowingResult& rslt)
{
	if(m_ReqID != id || !rslt.IsSuccess())
	{
		return;
	}
	m_OnRequestDone.ExecuteIfBound();
	m_OnRequestDone.Unbind();
}

void AMyPlayerPawn::BindOnCancel(const FVoidVoid& onCancel)
{
	m_OnCancelInteract = onCancel;
}

void AMyPlayerPawn::UnbindCancel()
{
	m_OnCancelInteract.Unbind();
}

void AMyPlayerPawn::StartBuildCam()
{
	m_DissolveCam->TargetArmLength = 1500; //1375

	m_DissolveCam->SetRelativeRotation(FRotator(-70, -45.f, 0.f)); //-45.f
}

void AMyPlayerPawn::SetCameraOffset(const FVector2D& delta)
{
	FVector2D NewLoc2D = delta.GetRotated(45);
	
	FVector NewLoc = -FVector(NewLoc2D.X, NewLoc2D.Y, 0);

	m_DissolveCam->AddRelativeLocation(NewLoc);

	FVector Loc = m_DissolveCam->GetComponentLocation();

	Loc.X = FMath::Clamp(Loc.X, -2700.f, 2700.f);
	
	Loc.Y = FMath::Clamp(Loc.Y, -2700.f, 2700.f);

	m_DissolveCam->SetWorldLocation(Loc);
}

void AMyPlayerPawn::EndBuildCam()
{
	m_DissolveCam->SetRelativeLocation(FVector(0.f));
	
	m_DissolveCam->TargetArmLength = 1300; //1375

	m_DissolveCam->SetRelativeRotation(FRotator(-50, -45.f, 0.f)); //-45.f
}

bool AMyPlayerPawn::IsLooting()
{
	return m_WaitInteractTimer.IsValid();
}

UCameraComponent* AMyPlayerPawn::GetCameraComp()
{
	return m_TopCamera;
}

float AMyPlayerPawn::PlayBaseAttackAnim()
{
	const TArray<FCompositeSection>& AnimAry = GetBaseAttackMontage()->CompositeSections;
	
	int RandIndex = FMath::RandRange(0, AnimAry.Num()-1);
	
	return PlayAnimMontage(GetBaseAttackMontage(), 1 * GetStat().m_AtkPerSec, AnimAry[RandIndex].SectionName);
}

float AMyPlayerPawn::TryAttack()
{
	if (IsAlive() && GetBaseAttackMontage() && m_fAttackCD < 0.f)
	{
		float AnimMongLen = PlayBaseAttackAnim();

		m_fAttackCD = FMath::Max(AnimMongLen - 0.1f,  0.15f);

		return m_fAttackCD;
	}

	return 0.f;
}

void AMyPlayerPawn::OnNotifyTrigger(const FName& id)
{
	if(id == TEXT("BaseAttack"))
	{
		if(!CheckTargetRange())
		{
			return;
		}
		if (IsRangeMode())
		{
			ShootBaseRangeAttack();	
		}
		else
		{
			DealBaseMeleeAttack();
		}
	}
	else
	{
		m_OnSkillTrigger.Broadcast(id);
	}
}

bool AMyPlayerPawn::TakeDmg(float amount, ACombatUnitPawn* attacker)
{
	if(!IsAlive())
	{
		return false;
	}
	UMyGameInstance::Get->m_PlayerStatManager->OnPlTookDmg(amount);
	
	if (m_bIsInvincible)
	{
		UMyLib::GetPlayerCon()->ShowInGameWorldText(TEXT("Immune"),this,ETextType::Immune);
		return false;
	}

	if(!Super::TakeDmg(amount, attacker))
	{
		return false;
	}
	UMyLib::GetCanvas()->GetScreenEffect()->ShowHittenFlash();

	m_LastAttacker = attacker;

	UMyGameInstance::Get->m_EquipManager->TakeDurDmg(1);
	
	return true;
}

void AMyPlayerPawn::TakeHeal(float v)
{
	UMyLib::GetPlayerCon()->ShowInGameWorldText(v,this,ETextType::PlayerTookHeal);

	UMyGameInstance::Get->m_PlayerStatManager->AddHp(v);
}

void AMyPlayerPawn::TakeInvincible(float d)
{
	m_bIsInvincible = true;
	
	GetWorldTimerManager().SetTimer(m_InvincTimer, this, &AMyPlayerPawn::StopInvincible, d, false);
}

void AMyPlayerPawn::StopInvincible()
{
	m_bIsInvincible = false;
	
	GetWorldTimerManager().ClearTimer(m_InvincTimer);
}

float AMyPlayerPawn::PlaySkillAnim(const FName& skillID)
{
	return 0.f;//No skill
}

void AMyPlayerPawn::OnDeathAnimEnd()
{
	Super::OnDeathAnimEnd();

	UMyGameInstance::Get->m_PlayerStatManager->OnPlayerDead(m_LastAttacker.Get());
}

bool AMyPlayerPawn::IsRange()
{
	return UMyGameInstance::Get->m_EquipManager->IsRangeStance();
}

bool AMyPlayerPawn::IsSneak() const
{
	return m_bIsSneaking;
}

bool AMyPlayerPawn::CheckTargetRange()
{
	if(!GetFocusedTarget<ACombatUnitPawn>())
	{
		return false; 
	}
	float DistSqr = FVector::DistSquared2D(GetActorLocation(), GetFocusedActorLocation());

	return DistSqr <= GetAttackRangeSqr();
}