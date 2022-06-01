#include "MonsterPawn.h"

#include "Components/MyMovement.h"
#include "Logics/AI/AI_Logic/AI_LogicBase.h"
#include "Logics/AI/AI_Sensor/Sensor_LogicBase.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"
#include "MyJrpg/Widgets/WidgetComponents/WidgetPawnInfoComp.h"
#include "MyJrpg/Widgets/WidgetComponents/WidgetSpeechBubbleComp.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& obj): Super(obj.SetDefaultSubobjectClass<UMyFlockSteering>(TEXT("m_Movement")))
{
	m_SoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio01"));
	m_SoundComp->SetupAttachment(RootComponent);
	m_SoundComp->SetAutoActivate(false);

	m_HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleHit02"));
	m_HitParticle->SetupAttachment(RootComponent);

	m_HitParticle->SetRelativeLocation(FVector(0, 0, 0.f));
	m_HitParticle->SetAutoActivate(false);
	m_HitParticle->SetRelativeScale3D(FVector(1));
	m_HitParticle->Deactivate();
	//PawnInfoWidget
	m_PawnInfo = CreateDefaultSubobject<UWidgetPawnInfoComp>(TEXT("PawnInfoWidget"));
	m_PawnInfo->SetupAttachment(m_Capsule);
	m_PawnInfo->SetVisibility(false);
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundHpBar(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/WB_PawnInfo.WB_PawnInfo_C'"));
	m_PawnInfo->SetWidgetClass(FoundHpBar.Class);
	//m_PawnInfo->SetDrawSize(FVector2D(150.f, 22.f));
	m_PawnInfo->SetDrawAtDesiredSize(true);
	FVector2D Pivot(0.5f, 0.5f);
	m_PawnInfo->SetPivot(Pivot);
	m_PawnInfo->SetWidgetSpace(EWidgetSpace::Screen);
	m_PawnInfo->SetRelativeLocation(FVector(0, 0, 130));
	m_PawnInfo->SetCanEverAffectNavigation(false);
	//m_SpeechBubbleComp
	m_SpeechBubbleComp = CreateDefaultSubobject<UWidgetSpeechBubbleComp>(TEXT("SpeechBubbleComp"));
	m_SpeechBubbleComp->SetupAttachment(m_Capsule);
	m_SpeechBubbleComp->SetVisibility(false);
	m_SpeechBubbleComp->SetDrawAtDesiredSize(true);
	m_SpeechBubbleComp->SetPivot(Pivot);
	m_SpeechBubbleComp->SetWidgetSpace(EWidgetSpace::Screen);
	m_SpeechBubbleComp->SetRelativeLocation(FVector(0, 0, 130));
	m_SpeechBubbleComp->SetCanEverAffectNavigation(false);
	//
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundHitEffect(
		TEXT("ParticleSystem'/Game/03_VisualEffect/P_Hit.P_Hit'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> FoundHitSound(
		TEXT("SoundWave'/Game/Sound/Fantasy_Game_Weapon_Impact.Fantasy_Game_Weapon_Impact'"));

	m_HitParticle->SetTemplate(FoundHitEffect.Object);

	m_SoundComp->SetSound(FoundHitSound.Object);
}

void AMonsterPawn::SetEntity(const FName& id,const FNpcUnitEntityRow& unitEntityRow)
{
	m_fAttackRange = unitEntityRow.m_fAtkRange;
	
	Super::SetEntity(id,unitEntityRow);

	float Z =  m_BodyMesh->Bounds.BoxExtent.Z;

	m_PawnInfo->SetRelativeLocation(FVector(0,0,Z));

	float HalfZ = (Z*0.5f) - GetCapsule()->GetScaledCapsuleHalfHeight();

	m_HitParticle->SetRelativeLocation(FVector(0,0,HalfZ));

	m_ShadowMeshComp->SetRelativeScale3D(FVector(unitEntityRow.m_fShadowScale));
	
	m_SpawnPoint = GetActorLocation();

	m_SpawnRot = GetActorRotation();

	m_fMaxHp = m_StatGroup.m_MaxHp;

	m_fExp = unitEntityRow.m_fExp;

	m_fGold = unitEntityRow.m_fGold;

	if(unitEntityRow.m_Bullet)
	{
		m_Pool = NewObject<UBulletPool>(this);
		
		m_Pool->InitPool(GetStat().m_Dmg,5,unitEntityRow.m_Bullet,this,unitEntityRow.m_fBulletScale);
	}

	m_bIsBoss = unitEntityRow.m_bIsBoss;

	m_TalkID = unitEntityRow.m_TalkID;

	CreateInventory();
}

void AMonsterPawn::SetReviveTime(float min, float max)
{
	m_fMinReviveTimer = min;

	m_fMaxReviveTimer = max;
}

void AMonsterPawn::OnNotifyTrigger(const FName& name)
{
	if(name == TEXT("BaseAttack"))
	{
		if(!GetFocusedTarget())
		{
			return;
		}
		
		if(m_Pool)
		{
			m_Pool->ShootBullet(TEXT("Trail_End_L"), GetFocusedTarget<ACombatUnitPawn>());
		}
		else
		{
			GetFocusedTarget<ACombatUnitPawn>()->TakeDmg(m_StatGroup.m_Dmg,this);	
		}
	}
	else
	{
		m_OnSkillTrigger.Broadcast(name);
	}
}

float AMonsterPawn::GetRewardExp() const
{
	return m_fExp;
}

float AMonsterPawn::GetRewardGold() const
{
	return m_fGold;
}

bool AMonsterPawn::IsBoss() const
{
	return m_bIsBoss;
}

const FName& AMonsterPawn::GetTalkID() const
{
	return m_TalkID;
}

UInventory* AMonsterPawn::GetInven()
{
	return m_Inven;
}

void AMonsterPawn::PlayHitFlash()
{
	FName TimeParamName = TEXT("StartTime");

	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_BodyMesh->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
}

void AMonsterPawn::PlayHittenSound()
{
	m_SoundComp->Play();
}

void AMonsterPawn::PlayHittenEffect()
{
	m_HitParticle->Activate(true);
}

void AMonsterPawn::Revive()
{
	return;
	SetActorHiddenInGame(false);
	
	SetActorLocation(m_SpawnPoint);

	SetActorRotation(m_SpawnRot);
	
	m_BodyMesh->bPauseAnims = false;
	
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//
	m_BodyMesh->SetVectorParameterValueOnMaterials(TEXT("EffectColor"), UKismetMathLibrary::Conv_LinearColorToVector(FLinearColor::White));
	//
	FName MaskParam = TEXT("Visibility");

	m_BodyMesh->SetScalarParameterValueOnMaterials(MaskParam, 1.0f);
	
	FName DurationParamName = TEXT("Duration");

	m_BodyMesh->SetScalarParameterValueOnMaterials(DurationParamName, 0.25f);

	FName TimeParamName = TEXT("StartTime");

	float TimeSec = 0;

	m_BodyMesh->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
	////
	PlayAnimMontage(m_EntityAsset->m_SpawnAnim);
	//
	float AnimLength = m_EntityAsset->m_SpawnAnim->GetPlayLength() - 0.4f;
	//22 05 03 :게임 디자인적으로 내구도가 존재하는데 몹이 리젠되면 손해 
	GetWorldTimerManager().SetTimer(m_DeathAnimTimer, this, &AMonsterPawn::OnReviveAnimEnd, AnimLength, false);
}

void AMonsterPawn::OnReviveAnimEnd()
{
	m_ShadowMeshComp->SetVisibility(true);

	m_Movement->SetComponentTickEnabled(true);

	if(m_AiSensor)
		m_AiSensor->SetSensingUpdatesEnabled(true);

	SetActorTickEnabled(true);

	m_StatGroup.m_Hp = m_fMaxHp;
}

void AMonsterPawn::CreateInventory()
{
	m_Inven = NewObject<UInventory>(this);
	m_Inven->Init(FGlobalVariable::MOB_INVEN);
	m_Inven->AddItem(FItemSpec(TEXT("Coin"), 10));
}

void AMonsterPawn::SetFocusedTarget(IFocusable* target)
{
	if (target)
	{
		switch (UMyGameInstance::Get->m_TeamKarma->GetUnitKarma(this))
		{
		case EKarma::Neutral:
			Speech(TEXT("처신 잘하라고"));
			break;
		case EKarma::Friendly:
			Speech(TEXT("좋은 아침이야~!"));
			break;
		case EKarma::Hate:
			Speech(TEXT("넌 뒤졌어"));
			break;
		}
	}
	Super::SetFocusedTarget(target);
}

void AMonsterPawn::SetReviveTimer()
{
	FTimerHandle m_ReviveHandle;
	
	GetWorldTimerManager().SetTimer(m_ReviveHandle,this,&AMonsterPawn::Revive, FMath::RandRange(m_fMinReviveTimer,m_fMaxReviveTimer), false);
}

void AMonsterPawn::Dead()
{
	m_ShadowMeshComp->SetVisibility(false);
	
	m_PawnInfo->SetVisibility(false);
	
	Super::Dead();

	UMyGameInstance::Get->m_GameRule->OnMonsterDead(this);

	m_SpeechBubbleComp->SetVisibility(false);
}

bool AMonsterPawn::TakeDmg(float amount, ACombatUnitPawn* attacker)
{
	if(!IsAlive())
	{
		return false;
	}
	
	if(!m_PawnInfo->IsVisible())
	{
		m_PawnInfo->SetVisibility(true);
		m_PawnInfo->SetPawnInfo(this);
	}

	if(!GetFocusedTarget())
	{
		amount *= 1.5f;
		SetFocusedTarget(attacker);
	}
	
	UMyGameInstance::Get->m_TeamKarma->DecreaseKarma(GetTeamID(),60);
	
	if(!Super::TakeDmg(amount, attacker))
	{
		return false;
	}

	PlayHitFlash();
	PlayHittenSound();
	PlayHittenEffect();
	m_PawnInfo->SetPawnInfo(this);
	return true;
}

void AMonsterPawn::Speech(FText text)
{
	m_SpeechBubbleComp->Speech(text);
}

void AMonsterPawn::Speech(FString text)
{
	m_SpeechBubbleComp->Speech(FText::FromString(text));
}