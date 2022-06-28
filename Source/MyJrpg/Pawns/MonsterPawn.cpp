#include "MonsterPawn.h"

#include "Components/MyMovement.h"
#include "Logics/AI/AI_Logic/AI_LogicBase.h"
#include "Logics/AI/AI_Logic/Logic_Flee.h"
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
	//
	//
	m_MeshLeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshLeftHand"));
	m_MeshLeftHand->SetupAttachment(m_BodyMesh);
	m_MeshLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshLeftHand->bCastDynamicShadow = false;
	m_MeshLeftHand->bAffectDynamicIndirectLighting = true;
	m_MeshLeftHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshLeftHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_MeshLeftHand->bReceivesDecals = false;
	//
	m_MeshRightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshRightHand"));
	m_MeshRightHand->SetupAttachment(m_BodyMesh);
	m_MeshRightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshRightHand->bCastDynamicShadow = false;
	m_MeshRightHand->bAffectDynamicIndirectLighting = true;
	m_MeshRightHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshRightHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_MeshRightHand->bReceivesDecals = false;
	//
	m_MeshBackHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshBackHand"));
	m_MeshBackHand->SetupAttachment(m_BodyMesh);
	m_MeshBackHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshBackHand->bCastDynamicShadow = false;
	m_MeshBackHand->bAffectDynamicIndirectLighting = true;
	m_MeshBackHand->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshBackHand->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_MeshBackHand->bReceivesDecals = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D>
	FoundIcon(TEXT("Texture2D'/Game/Sprites/UI/Arrow_Ene.Arrow_Ene'"));
	m_MinimapIcon = FoundIcon.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D>
	FoundIcon2(TEXT("Texture2D'/Game/Sprites/UI/Arrow_NAtk.Arrow_NAtk'"));
	m_IconNonAttack = FoundIcon2.Object;
}

void AMonsterPawn::BeginPlay()
{
	Super::BeginPlay();
	AttachWeapons();
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

	if(unitEntityRow.m_Bullet)
	{
		m_Pool = NewObject<UBulletPool>(this);
		
		m_Pool->InitPool(GetStat().m_Dmg,5,unitEntityRow.m_Bullet,this,unitEntityRow.m_fBulletScale);
	}

	m_bIsBoss = unitEntityRow.m_bIsBoss;

	m_TalkID = unitEntityRow.m_TalkID;

	if(unitEntityRow.m_TakeHitEffect)
	{
		m_HitParticle->SetTemplate(unitEntityRow.m_TakeHitEffect);
	}

	if(unitEntityRow.m_TakeHitSound)
	{
		m_SoundComp->SetSound(unitEntityRow.m_TakeHitSound);
	}

	if(m_EntityAsset->m_Attach.m_LeftMesh)
	{
		m_MeshLeftHand->SetStaticMesh(m_EntityAsset->m_Attach.m_LeftMesh);
	}
	if(m_EntityAsset->m_Attach.m_RightMesh)
	{
		m_MeshRightHand->SetStaticMesh(m_EntityAsset->m_Attach.m_RightMesh);
	}
	if(m_EntityAsset->m_Attach.m_BackMesh)
	{
		m_MeshBackHand->SetStaticMesh(m_EntityAsset->m_Attach.m_BackMesh);
	}
}

void AMonsterPawn::AttachWeapons()
{
	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
	m_MeshLeftHand->AttachToComponent(m_BodyMesh, Rules, TEXT("LeftHandSocket"));
	m_MeshRightHand->AttachToComponent(m_BodyMesh, Rules, TEXT("RightHandSocket"));
	m_MeshBackHand->AttachToComponent(m_BodyMesh, Rules, TEXT("BackSocket"));
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

void AMonsterPawn::SetHp(int hp)
{
	m_StatGroup.m_Hp = hp;
	
	m_PawnInfo->SetPawnInfo(this);
}

void AMonsterPawn::SetInven(UInventory* inven)
{
	if(!inven)
	{
		CreateInventory();
		
		return;
	}
	m_Inven = inven;
}

void AMonsterPawn::SetDeadBody()
{
	if(m_Pool)
		m_Pool->HideAll();
	if(m_AiSensor)
		m_AiSensor->SetSensingUpdatesEnabled(false);
	SetFocusedTarget(nullptr);
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Movement->SetComponentTickEnabled(false);
	m_ShadowMeshComp->SetVisibility(false);
	m_PawnInfo->SetVisibility(false);
	m_SpeechBubbleComp->SetVisibility(false);
	
	GetSkMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetSkMesh()->SetAnimation(m_EntityAsset->m_DeathMontage);
	float AnimLength = m_EntityAsset->m_DeathMontage->GetPlayLength() * 0.85f;
	GetSkMesh()->SetPosition(AnimLength);
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

void AMonsterPawn::CreateInventory()
{
	m_Inven = NewObject<UInventory>(UMyGameInstance::Get);

	FText InvenT = NSLOCTEXT("AMonsterPawn","'s Inven","의 소지품");

	FString FormatStr = FString::Printf(TEXT("%s%s"),*m_PawnName.ToString(),*InvenT.ToString());
	
	m_Inven->Init(FGlobalVariable::MOB_INVEN, FText::FromString(FormatStr));
}

void AMonsterPawn::Dead()
{
	m_ShadowMeshComp->SetVisibility(false);
	
	m_PawnInfo->SetVisibility(false);
	
	Super::Dead();

	m_SpeechBubbleComp->SetVisibility(false);
}

void AMonsterPawn::OnDeathAnimEnd()
{
	Super::OnDeathAnimEnd();
	
	GetSkMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
	if(!IsAlive())
	{
		m_SpeechBubbleComp->Hide();
		return;
	}
	m_SpeechBubbleComp->Speech(text);
}

void AMonsterPawn::Speech(FString text)
{
	Speech(FText::FromString(text));
}

void AMonsterPawn::OnInteract()
{
	UMyLib::GetPlayer()->SetInteracting(true);
	
	UMyLib::GetPlayer()->RequestInteract(this,FVoidVoid::CreateUObject(this,&AMonsterPawn::OnRequestMoveDone),25);
} 

FText AMonsterPawn::GetTextInteract()
{
	return NSLOCTEXT("AMonsterPawn","Loot","살펴보기");
}

bool AMonsterPawn::IsInteractable()
{
	if(IsAlive())
	{
		return true;
	}
	return m_Inven && !m_Inven->IsInvenEmpty();
}

void AMonsterPawn::OnRequestMoveDone()
{
	UMyLib::GetCanvas()->StartPickPocket(this);
}

void AMonsterPawn::SetIcon()
{
	if(m_AiFsm->GetClass() == ULogic_Flee::StaticClass() || !UMyGameInstance::Get->m_TeamKarma->IsFoe(this))
	{
		m_IconComp->SetIcon(m_IconNonAttack);	
	}
	else
	{
		m_IconComp->SetIcon(m_MinimapIcon);
	}
}
