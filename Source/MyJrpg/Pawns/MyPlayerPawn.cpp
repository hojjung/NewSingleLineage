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

	m_bIsSkillUsing = false;
	m_bCanMoveInSkill = false;
	m_bIsInvincible = false;
	m_bIsSneaking = false;
	m_bUseFsmTick = false;

	m_Light = CreateDefaultSubobject<UPointLightComponent>("m_Light");
	m_Light->SetupAttachment(m_Capsule);
	m_Light->SetRelativeLocation(FVector(0,0,140));
	m_Light->CastShadows = false;
	m_Light->LightColor = FColor(255,204,127);
	m_Light->Intensity = 2000;
	
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-45, -45.f, 0.f)); //-45.f
	m_DissolveCam->TargetArmLength = 1500; //1375
	m_DissolveCam->m_SocketOffset = FVector(0,0,-50);
	m_DissolveCam->CameraLagSpeed=30;
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera00"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 65.f;
	//
	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	m_bOnceMoveLock = false;
	//
	m_ShadowMeshComp->SetRelativeScale3D(FVector(4));

	m_bIsInteracting = false;

	SetAttackRange(250);
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

	FStatGroup DefaultStat;
	DefaultStat.m_nAccu = 10;
	DefaultStat.m_nAvoid = 0;
	DefaultStat.m_MaxHp = 100;
	DefaultStat.m_Dmg = 4;
	DefaultStat.m_AtkPerSec = 0.8f;
	DefaultStat.m_DmgReduce = 0;
	DefaultStat.m_CriPer = 0.1f;
	DefaultStat.m_CriDmg = 1.5f;
	DefaultStat.m_MoveSpeed = FGlobalVariable::HERO_DEFAULT_SPEED;
	
	UMyGameInstance::Get->m_PlayerStatManager->SetBaseStat(DefaultStat);
	
	UMyGameInstance::Get->m_PlayerStatManager->UpdateStat();
	
	m_StatGroup.m_Hp = m_StatGroup.m_MaxHp;
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
	if (m_bIsSkillUsing)
	{
		return ;
	}
	ACombatUnitPawn* FocusActor = GetFocusedTarget<ACombatUnitPawn>();
	if(!FocusActor)
	{
		TryAttack();
		return;
	}
	
	RequestInteract(FocusActor, FVoidVoid::CreateUObject(this, &AMyPlayerPawn::TryAttack_External),GetAttackRange());
}

void AMyPlayerPawn::TryAttack_External()
{
	TryAttack();
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
}

void AMyPlayerPawn::ShowPopupText(float nbr, ETextType t)
{
	//not use
}

void AMyPlayerPawn::ShowIndicator(IFocusable* target)
{
	if(!target)
	{
		m_FocusIndicator->SetActorHiddenInGame(true);
		return;
	}
	m_FocusIndicator->SetActorHiddenInGame(false);

	INavAgentInterface* FocusActor = Cast<INavAgentInterface>(target);

	FVector Loc = FocusActor->GetNavAgentLocation();

	FAttachmentTransformRules Rule(EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,false);
	
	m_FocusIndicator->AttachToActor(Cast<AActor>(target), Rule);

	m_FocusIndicator->SetActorLocation(Loc);
}

void AMyPlayerPawn::SetFocusedTarget(IFocusable* target)
{
	if(m_bIsInteracting)
	{
		return;
	}
	Super::SetFocusedTarget(target);

	m_OnFocus.Broadcast(Cast<IFocusable>(m_FocusedTarget.GetObject()));
	
	ShowIndicator(target);
}

void AMyPlayerPawn::PlayTookHitMontage()
{
	//not use
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
	float HpPercent = GetHpPercent();
	
	m_StatGroup = stat_group;

	m_StatGroup.m_Hp = m_StatGroup.m_MaxHp * HpPercent;

	m_Movement->MaxSpeed = m_StatGroup.m_MoveSpeed;
}

void AMyPlayerPawn::DealBaseMeleeAttack()
{
	if(!GetFocusedTarget())
	{
		return ;
	}

	ACombatUnitPawn* Pawn =  Cast<ACombatUnitPawn>(GetFocusedTarget());

	if(!Pawn)
	{
		return ;
	}
		
	UMyGameInstance::Get->m_PlayerStatManager->OnAttack(this);

	Pawn->TakeDmg(m_StatGroup.m_Dmg,this);

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
		m_Movement->m_fSpeedMultiple = 0.65f;
		HideWeapon();
	}
	else
	{
		m_Movement->m_fSpeedMultiple = 1.f;
		ShowWeapon();
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

void AMyPlayerPawn::WaitInteract(UAnimMontage* am, float interactTime, const FVoidVoid& delegate)
{
	SetInteracting(true);
	PlayAnimMontage(am);
	GetWorldTimerManager().SetTimer(m_WaitInteractTimer,delegate,interactTime,false);
	UMyLib::GetCanvas()->GetWaitInteract()->ShowInteract(interactTime);
}

void AMyPlayerPawn::RequestInteract(AActor* target, const FVoidVoid& delegate ,float r)
{
	FPathFollowingRequestResult Result = MoveToActor(target, r);
	
	if(Result.Code == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		HomingRotateToTarget(0);
		delegate.ExecuteIfBound();
		return;
	}
	
	m_ReqID = MoveToActor(target, r).MoveId;

	m_OnRequestDone = delegate;
}

void AMyPlayerPawn::BindOnCancel(const FVoidVoid& onCancel)
{
	m_OnCancelInteract = onCancel;
}

void AMyPlayerPawn::UnbindCancel()
{
	m_OnCancelInteract.Unbind();
}

FVector2D AMyPlayerPawn::GetCameraOffset()
{
	return m_Offset;
}

void AMyPlayerPawn::SetCameraOffset(const FVector2D& vector_2d)
{
	m_Offset = vector_2d;

	FVector2D NewLoc2D = m_Offset.GetRotated(45);
	
	FVector NewLoc = -FVector(NewLoc2D.X,NewLoc2D.Y, 0);
	
	m_DissolveCam->SetRelativeLocation(NewLoc);
}

void AMyPlayerPawn::ClearCameraOffset()
{
	m_Offset = FVector2D(0);
	
	m_DissolveCam->SetRelativeLocation(FVector(0,0,0));
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

void AMyPlayerPawn::OnNotifyTrigger(const FName& name)
{
	if(name == TEXT("BaseAttack"))
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
		m_OnSkillTrigger.Broadcast(name);
	}
}

bool AMyPlayerPawn::TakeDmg(float amount, ACombatUnitPawn* attacker)
{
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

	UMyGameInstance::Get->m_PlayerStatManager->OnPlHpChanged(this);

	m_LastAttacker = attacker;
	
	return true;
}

void AMyPlayerPawn::TakeHeal(float v)
{
	UMyLib::GetPlayerCon()->ShowInGameWorldText(v,this,ETextType::PlayerTookHeal);

	m_StatGroup.m_Hp += v;

	m_StatGroup.m_Hp = FMath::Min(m_StatGroup.m_Hp,m_StatGroup.m_MaxHp);

	UMyGameInstance::Get->m_PlayerStatManager->OnPlHpChanged(this);
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

void AMyPlayerPawn::CreateRangeBullet(UParticleSystem* effectBullet)
{
	RemoveBulletPool();
	
	m_Pool = NewObject<UBulletPool>(this);
	
	m_Pool->InitPool(GetStat().m_Dmg,12,effectBullet,this,0.3f);
}

void AMyPlayerPawn::RemoveBulletPool()
{
	if(!m_Pool)
	{
		return;
	}
	m_Pool->KillAll();
	m_Pool = nullptr;
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
	if(!GetFocusedTarget<>())
	{
		return false; 
	}
	float DistSqr = FVector::DistSquared(GetActorLocation(), GetFocusedActorLocation());

	return DistSqr <= GetAttackRangeSqr();
}
