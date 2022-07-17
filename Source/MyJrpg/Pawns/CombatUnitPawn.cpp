#include "CombatUnitPawn.h"
#include "MyPlayerPawn.h"
#include "Components/MyMovement.h"
#include "Logics/AI/AI_Logic/AI_LogicBase.h"
#include "Logics/AI/AI_Sensor/Sensor_LogicBase.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/HumanAsset.h"

ACombatUnitPawn::ACombatUnitPawn(const FObjectInitializer& objInit):Super(objInit)
{
	m_fAttackCD=0;
   	m_fHitAnimCD = -1.f;
	SetAttackRange(200);

	m_bIsRotateable = true;
	m_bUseFsmTick = true;
	m_bCanUseSkill = true;
	//
	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
			TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(GetSkMesh());
	m_ShadowMeshComp->SetRelativeLocation(FVector(0,0,5.f));
	m_ShadowMeshComp->SetRelativeScale3D(FVector(1));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
	m_ShadowMeshComp->bReceivesDecals = false;

	m_BulletTarget = CreateDefaultSubobject<USceneComponent>(TEXT("m_BulletTarget"));
	m_BulletTarget->SetupAttachment(RootComponent);
	//
	m_IconComp = CreateDefaultSubobject<UMinimapIconComp>("m_NonRotComp");
	m_IconComp->SetupAttachment(RootComponent);
}

void ACombatUnitPawn::SetIcon()
{
	m_IconComp->SetIcon(m_MinimapIcon);
}

bool ACombatUnitPawn::TryHit(const FStatGroup& other)
{
	int Accu = other.m_nAccu + 1;

	int Avoid = GetStat().m_nAvoid + 1;

	int RandAccu = FMath::RandRange(0,Accu);

	int RandAvoid = FMath::RandRange(0,Avoid);

	return RandAccu >= RandAvoid;
}

void ACombatUnitPawn::HomingRotateToTarget(float speedTime)
{
	if (!GetFocusedTarget() || !m_bIsRotateable || !IsAlive())
	{
		return;
	}

	FRotator NewRot = GetActorRotation();

	if(speedTime <= 0)
	{
		NewRot.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cast<AActor>(GetFocusedTarget())->GetActorLocation()).Yaw;
	}
	else
	{
		NewRot.Yaw = UKismetMathLibrary::RInterpTo(NewRot, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cast<AActor>(GetFocusedTarget())->GetActorLocation()),GetWorld()->GetDeltaSeconds(), speedTime).Yaw;
	}

	SetActorRotation(NewRot);
}

void ACombatUnitPawn::SetRotateAble(bool b)
{
	m_bIsRotateable = b;
}

float ACombatUnitPawn::PlayBaseAttackAnim()
{
	const TArray<FCompositeSection>& AnimAry = GetBaseAttackMontage()->CompositeSections;
	
	int RandIndex = FMath::RandRange(0, AnimAry.Num()-1);
	
	return PlayAnimMontage(GetBaseAttackMontage(), 1, AnimAry[RandIndex].SectionName);
}

UAnimMontage* ACombatUnitPawn::GetBaseAttackMontage()
{
	return m_EntityAsset->GetBaseAtkAnim();
}

void ACombatUnitPawn::ShowPopupText(float nbr, ETextType t)
{
	UMyLib::GetPlayerCon()->ShowInGameWorldText(nbr,this,t);
}

void ACombatUnitPawn::SetEntity(const FName& id, const FNpcUnitEntityRow& unitEntityRow)
{
	Super::SetEntity(id, unitEntityRow);
	
	if(unitEntityRow.m_ClassAI_Logic->IsValidLowLevel())
	{
		m_AiFsm = NewObject<UAI_LogicBase>(this,unitEntityRow.m_ClassAI_Logic);
		m_AiFsm->Init(this);
	}

	if(unitEntityRow.m_ClassAI_Sensor->IsValidLowLevel())
	{
		m_AiSensor = NewObject<USensor_LogicBase>(this,unitEntityRow.m_ClassAI_Sensor);
		m_AiSensor->Init(this);
	}

	float Height = GetCapsule()->Bounds.BoxExtent.Z * 0.5f;
	
	m_BulletTarget->SetRelativeLocation(FVector(0,0,Height));

	m_TeamID = unitEntityRow.m_FriendTeamID;

	UHumanAsset* HumanAsset = Cast<UHumanAsset>(unitEntityRow.m_UnitDataAsset.Get());
	if(HumanAsset)
	{
		m_Stance = HumanAsset->m_StanceType;;
	}
	SetIcon();
}

void ACombatUnitPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsAlive())
	{
		return;
	}

	m_fHitAnimCD -= DeltaSeconds;
	
	m_fAttackCD -= DeltaSeconds;

	if (m_AiFsm && m_bUseFsmTick)
	{
		m_AiFsm->Tick(DeltaSeconds);
	}
}

void ACombatUnitPawn::SetFocusedTarget(IFocusable* target)
{
	m_FocusedTarget.SetInterface(target);
	m_FocusedTarget.SetObject(Cast<UObject>(target));
}

float ACombatUnitPawn::TryAttack()
{
	if (GetBaseAttackMontage() && m_fAttackCD < 0.f)
	{
		float AnimMongLen = PlayBaseAttackAnim();

		m_fAttackCD = FMath::Max(AnimMongLen - 0.1f,  1.f / GetStat().m_AtkPerSec);

		return m_fAttackCD;
	}

	return 0.f;
}

void ACombatUnitPawn::PlayTookHitMontage()
{
	if (m_EntityAsset->m_TookHitMontage && m_fHitAnimCD < 0.f)
	{
		SetRotateAble(true);
		PlayAnimMontage(m_EntityAsset->m_TookHitMontage, 1);
	
		m_fHitAnimCD = FMath::RandRange(5.5f, 25.f);
	}
}

void ACombatUnitPawn::SetAttackRange(float range)
{
	m_fAttackRange = range;

	m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
}

float ACombatUnitPawn::GetAttackRange()
{
	return m_fAttackRange;
}

float ACombatUnitPawn::GetAttackRangeSqr()
{
	return m_fAttackRangeSqr;
}

FVector ACombatUnitPawn::GetFocusedActorLocation() const
{
	return GetFocusedTarget<AActor>()->GetActorLocation();
}

FRotator ACombatUnitPawn::GetFocusedActorRotation() const
{
	return GetFocusedTarget<AActor>()->GetActorRotation();
}

void ACombatUnitPawn::SetEnableFsm(bool useFsm)
{
	m_bUseFsmTick = useFsm;
}

void ACombatUnitPawn::PlayDeathAnim(float rate)
{
	if(m_EntityAsset->m_DeathMontage)
	{
		float AnimLength = PlayAnimMontage(m_EntityAsset->m_DeathMontage, rate) * 0.85f;

		GetWorldTimerManager().SetTimer(m_DeathAnimTimer, this, &ACombatUnitPawn::OnDeathAnimEnd, AnimLength, false);
	}
	else
	{
		OnDeathAnimEnd();
	}
}

void ACombatUnitPawn::TakeStun(float duration)
{
	ClearStopMoveDelegate();
	StopMove();
	GetSkMesh()->bPauseAnims = true;
	m_Movement->SetActive(false);

	GetWorldTimerManager().SetTimer(m_StunTimer, this, &ACombatUnitPawn::StopStun, duration, false);
}

void ACombatUnitPawn::StopStun()
{
	GetSkMesh()->bPauseAnims = false;
	m_Movement->SetActive(true);

	GetWorldTimerManager().ClearTimer(m_StunTimer);
}

void ACombatUnitPawn::TakeSilence(float duration)
{
	m_bCanUseSkill = false;
	GetWorldTimerManager().SetTimer(m_SilenceTimer, this, &ACombatUnitPawn::StopSilence, duration, false);
}

void ACombatUnitPawn::StopSilence()
{
	m_bCanUseSkill = true;
	GetWorldTimerManager().ClearTimer(m_SilenceTimer);
}

bool ACombatUnitPawn::IsUseFsm()
{
	return m_bUseFsmTick;
}

FSkillTrigger& ACombatUnitPawn::GetSkillTriggerDelegate()
{
	return m_OnSkillTrigger;
}

const FStatGroup& ACombatUnitPawn::GetStat() const
{
	return m_StatGroup;
}

bool ACombatUnitPawn::IsRangeMode()
{
	return  m_Pool != nullptr;
}

USceneComponent* ACombatUnitPawn::GetBulletTarget()
{
	return m_BulletTarget;
}

const TArray<AActor*>& ACombatUnitPawn::GetTraceIgnoredActors() const
{
	return m_AryIgnores;
}

const TArray<TEnumAsByte<EObjectTypeQuery>>& ACombatUnitPawn::GetTraceObjTypes() const
{
	return m_AryTargetingObjectType;
}

const FName& ACombatUnitPawn::GetTeamID() const
{
	return m_TeamID;
}

bool ACombatUnitPawn::IsSneak() const
{
	return false;
}

EPathFollowingRequestResult::Type ACombatUnitPawn::ChaseTarget()
{
	return MoveToActor(GetFocusedTarget<AActor>());
}

EStanceType ACombatUnitPawn::GetStance()
{
	return m_Stance; 
}

bool ACombatUnitPawn::IsLooting()
{
	return false;
}

UMinimapIconComp* ACombatUnitPawn::GetIconMeshComp()
{
	return m_IconComp;
}

void ACombatUnitPawn::Dead()
{
	if(m_Pool)
	{
		m_Pool->HideAll();
	}
	
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetFocusedTarget(nullptr);

	m_Movement->SetComponentTickEnabled(false);

	if(m_AiSensor)
		m_AiSensor->SetSensingUpdatesEnabled(false);

	PlayDeathAnim();
}

void ACombatUnitPawn::OnDeathAnimEnd()
{
	m_BodyMesh->bPauseAnims = true;
	
	SetActorTickEnabled(false);
	
	GetWorldTimerManager().ClearTimer(m_DeathAnimTimer);
}

float ACombatUnitPawn::GetCriticalDmg(float amount)
{
	float RandCri = FMath::RandRange(0,1);

	if (RandCri >= GetStat().m_CriPer)
	{
		return 0;
	}

	return amount * GetStat().m_CriDmg; 
}

float ACombatUnitPawn::GetReductDmg(float amount)
{
	return FMath::Max<float>(amount - GetStat().m_DmgReduce, 0);
}

float ACombatUnitPawn::GetRandomDmg(float origin)
{
	float Rand = FMath::RandRange(0.80f,1.2f);

	return origin * Rand;
}

bool ACombatUnitPawn::TakeDmg(float amount, ACombatUnitPawn* attacker)
{
	ETextType TextType = ETextType::NormalRight;
	
	if(attacker && !TryHit(attacker->GetStat()))
	{
		ShowPopupText(0,ETextType::Miss);
		return false;
	}

	if(attacker)
	{
		amount = attacker->GetRandomDmg(amount);
		
	}

	if (attacker)
	{
		float CriDmg = attacker->GetCriticalDmg(amount);

		if (CriDmg)
		{
			amount = CriDmg;
		}
	}

	float ReducDmg = GetReductDmg(amount);

	if (ReducDmg <= 0)
	{
		ShowPopupText(0,TextType);
		return false;
	}
	ShowPopupText(amount,TextType);
	
	SubDmgFromHp(ReducDmg);

	if(!IsAlive())
	{
		Dead();
	}
	else
	{
		PlayTookHitMontage();	
	}

	return true;
}

void ACombatUnitPawn::OnNotifyTrigger(const FName& name)
{
	if(name == TEXT("BaseAttack"))
	{
		if(!GetFocusedTarget())
		{
			return;
		}

		ACombatUnitPawn* Pawn =  Cast<ACombatUnitPawn>(GetFocusedTarget());

		if(!Pawn)
		{
			return;
		}

		Pawn->TakeDmg(GetStat().m_Dmg,this);
	}
	else
	{
		m_OnSkillTrigger.Broadcast(name);
	}
}

float ACombatUnitPawn::GetHpPercent() const
{
	return  (float)GetStat().m_Hp / (float)GetStat().m_MaxHp;
}

int ACombatUnitPawn::GetHp()
{
	return  GetStat().m_Hp;
}
void ACombatUnitPawn::SubDmgFromHp(float dmg)
{
	m_StatGroup.m_Hp -= dmg;
}
bool ACombatUnitPawn::IsAlive()
{
	return m_StatGroup.m_Hp > 0;
}