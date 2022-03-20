#include "MyPlayerPawn.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Logics/PlAttchActorManage.h"
#include "Logics/PlayerAnimInst.h"
#include "Logics/PlSkillAuto.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Animations/MyAnimInstance.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
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

	m_Light = CreateDefaultSubobject<UPointLightComponent>("m_Light");
	m_Light->SetupAttachment(m_Capsule);
	m_Light->SetRelativeLocation(FVector(0,0,140));
	m_Light->CastShadows = false;
	m_Light->LightColor = FColor(255,204,127);
	m_Light->Intensity = 2000;
	
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-52, -45.f, 0.f)); //-45.f
	m_DissolveCam->TargetArmLength = 2800; //1375
	m_DissolveCam->m_SocketOffset = FVector(0,0,-250.0f);
	m_DissolveCam->CameraLagSpeed=30;
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera00"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 20.f;
	//
	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	m_bOnceMoveLock = false;
	//
	m_ShadowMeshComp->SetRelativeScale3D(FVector(4));
}

void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_AryIgnores.Reset();
	
	m_AryIgnores.Add(this);

	m_DissolveCam->SetActive(true);

	const FPlayerUnitEntityRow* UnitRow = UUnitEntityData::GetPlayerUnitTable->FindRow<FPlayerUnitEntityRow>(TEXT("Player01"), "");

	SetPlayerEntity(*UnitRow);
}

void AMyPlayerPawn::SetPlayerEntity(const FPlayerUnitEntityRow& unitEntityRow)
{
	m_fRangeAttackRange = unitEntityRow.m_fRangeRange;

	m_fAttackRange = unitEntityRow.m_fMeleeRange;
	
	LoadSetSkMeshAnim(unitEntityRow.m_UnitDataAsset);

	if (m_AnimInst)
	{
		m_AnimInst->KillAll();
	}
	m_AnimInst = NewObject<UPlayerAnimInst>(this);
	m_AnimInst->Init(unitEntityRow,this);

	m_AttchActorMng = NewObject<UPlAttchActorManage>(this);
	m_AttchActorMng->Init(this);

	ULogic_Player* Player = NewObject<ULogic_Player>(this,ULogic_Player::StaticClass());

	m_AiFsm = Player;

	m_AiFsm->Init(this);
	
	m_AiSensor = NewObject<USensor_Player>(this,USensor_Player::StaticClass());
	
	m_AiSensor->Init(this);

	UMyGameInstance::Get->m_PlayerStatManager->SetBaseStat(unitEntityRow.m_StatTable);
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

		FVector Loc = GetCapsule()->GetComponentLocation();

		FVector Delta = (m_DeltaX+m_DeltaY).GetUnsafeNormal() * 100.0f;

		MoveToLocation(Loc+Delta,0);

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
}

bool AMyPlayerPawn::IsInputMoving()
{
	return !m_Input.IsZero();
}

void AMyPlayerPawn::SetPlayerSkMeshDefault()
{
	LoadSetSkMeshAnim(m_EntityAsset.Get());
}

float AMyPlayerPawn::TryAttack()
{
	if (m_bIsSkillUsing)
	{
		return 0;
	}

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

void AMyPlayerPawn::SetPlayerAsset(FName keyId)
{
	const FPlayerUnitEntityRow* UnitRow = UUnitEntityData::GetPlayerUnitTable->FindRow<FPlayerUnitEntityRow>(keyId, "");
	
	LoadSetSkMeshAnim(UnitRow->m_UnitDataAsset);
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

const TArray<AActor*>& AMyPlayerPawn::GetTraceIgnoredActors() const
{
	return m_AryIgnores;
}

const TArray<TEnumAsByte<EObjectTypeQuery>>& AMyPlayerPawn::GetTraceObjTypes() const
{
	return m_AryTargetingObjectType;
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
}

void AMyPlayerPawn::ShootBaseRangeAttack()
{
	m_Pool->ShootBullet(TEXT("Trail_End_L"),GetFocusedTarget());
}

UAnimMontage* AMyPlayerPawn::GetBaseAttackMontage()
{
	return m_AnimInst->GetBasAttackAnim();
}

void AMyPlayerPawn::OnNotifyTrigger(const FName& name)
{
	if(name == TEXT("BaseAttack"))
	{
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

void AMyPlayerPawn::AddAtkRange(float r)
{
	m_fAddAtkRange += r;
}

void AMyPlayerPawn::SubAtkRange(float r)
{
	m_fAddAtkRange -= r;
}

float AMyPlayerPawn::GetAttackRange()
{
	return m_fAttackRange + m_fAddAtkRange;
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
	return PlayAnimMontage(m_AnimInst->GetSkillAnim(skillID));
}

float AMyPlayerPawn::GetRangeRange()
{
	return m_fRangeAttackRange + m_fAddAtkRange;
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
