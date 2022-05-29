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

	m_fAttackRange = 300;
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
			StopAnimMontage();
		
		ClearStopMoveDelegate();
		
		m_Movement->SetActive(true);

		SetInteracting(false);

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

float AMyPlayerPawn::TryAttack()
{
	if (m_bIsSkillUsing)
	{
		return 0;
	}
	HomingRotateToTarget(6);
	return Super::TryAttack();
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

	AActor* FocusActor = Cast<AActor>(target);

	FVector Loc = FocusActor->GetActorLocation();

	float H =  target->GetBoundHalfHeight();

	Loc.Z -= H;
	
	FAttachmentTransformRules Rule(EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,false);
	
	m_FocusIndicator->AttachToActor(FocusActor, Rule);

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
	PlayAnimMontage(am);
	SetInteracting(true);
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle,delegate,interactTime,false);
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