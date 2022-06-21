// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeBase.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

ATreeBase::ATreeBase()
{
	PrimaryActorTick.bCanEverTick = true; 
	
	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule00"));
	m_Capsule->InitCapsuleSize(34.0f, 40);
	m_Capsule->SetCollisionProfileName(TEXT("BlockAll"));
	m_Capsule->CanCharacterStepUpOn = ECB_No;
	m_Capsule->SetShouldUpdatePhysicsVolume(false);
	m_Capsule->SetCanEverAffectNavigation(true);
	m_Capsule->bDynamicObstacle = false;
	m_Capsule->AreaClass = nullptr;
	m_Capsule->SetMobility(EComponentMobility::Movable);
	m_Capsule->bReceivesDecals = false;
	RootComponent = m_Capsule;

	m_MeshTree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTree"));
	m_MeshTree->CanCharacterStepUpOn = ECB_No;
	m_MeshTree->SetCanEverAffectNavigation(false);
	m_MeshTree->SetupAttachment(m_Capsule);
	m_MeshTree->bReceivesDecals = false;

	m_MeshTree->SetMobility(EComponentMobility::Movable);
	m_MeshTree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshTree->SetCollisionProfileName(TEXT("PhysicsActor"));
	m_MeshTree->CastShadow = false;
	m_MeshTree->bSelfShadowOnly = false;
	m_MeshTree->SetRelativeLocation(FVector(0,0,-88));
	

	m_MeshTrunk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTrunk"));
	m_MeshTrunk->CanCharacterStepUpOn = ECB_No;
	m_MeshTrunk->SetCanEverAffectNavigation(false);
	m_MeshTrunk->SetupAttachment(m_Capsule);
	m_MeshTrunk->bReceivesDecals = false;

	m_MeshTrunk->SetMobility(EComponentMobility::Movable);
	m_MeshTrunk->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshTrunk->CastShadow = false;
	m_MeshTrunk->bSelfShadowOnly = false;
	m_MeshTrunk->SetRelativeLocation(FVector(0,0,-88));

	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(RootComponent);
	m_ShadowMeshComp->SetMobility(EComponentMobility::Movable);
	m_ShadowMeshComp->SetRelativeScale3D(FVector(6.5f));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
	m_ShadowMeshComp->SetRelativeLocation(FVector(0,0,-88));
	m_ShadowMeshComp->bReceivesDecals = false;
	m_nTreeHp = 3;
}
//(X=0.000005,Y=20.000000,Z=-10.000000) btm
//
void ATreeBase::SetEntity(const FName& id, const FGatherDataRow& data, AMyPlayerPawn* pl, const int *hpPtr )
{
	m_ID = id;
	
	m_DataRow = &data;

	if(hpPtr)
	{
		m_nTreeHp = *hpPtr; 
	}
	else
	{
		m_nTreeHp = m_DataRow->m_nTreeHp;
	}
	
	m_Player = pl;

	m_GatherAsset = UMyAssetManager::Get()->LoadGatherAsset(m_DataRow->m_GatherAsset);

	m_MeshTree->SetStaticMesh(m_GatherAsset->m_TopMesh);

	m_MeshTrunk->SetStaticMesh(m_GatherAsset->m_BtmMesh);

	StartDeathEffectMaterial(m_DataRow->m_fDisappearDelay);

	m_MeshTree->SetRelativeScale3D(FVector(m_DataRow->m_fTopMeshScale));
	
	m_MeshTrunk->SetRelativeScale3D(FVector(m_DataRow->m_fBtmMeshScale));

	m_bUsePhysics = m_DataRow->m_bUsePhysics;
}

void ATreeBase::OnInteract()
{
	if(m_nTreeHp<=0)
	{
		return;
	}
	m_Player->BindOnCancel(FVoidVoid::CreateUObject(this, &ATreeBase::OnHarvestMotionDone));
	m_Player->SetInteracting(true);
	m_Player->RequestInteract(this,FVoidVoid::CreateUObject(this,&ATreeBase::OnArrived),90);
}

void ATreeBase::OnArrived()
{
	m_Player->PlayAnimMontage(m_GatherAsset->m_AnimGatherMotion);

	for(const FName& ToolID : m_GatherAsset->m_AryGatherToolID)
	{
		m_CrntToolID = m_Player->TryShowAxe(ToolID);
		if(m_CrntToolID)
		{
			break;
		}
	}
}

void ATreeBase::OnTakeChopping()
{
	const FStatGroup& StatEquip = UMyLib::GetItemData(m_CrntToolID->m_ID).m_EquipStats;
		
	m_nTreeHp -= StatEquip.m_Dmg;
		
	UMyGameInstance::Get->m_PlayerStatManager->AddExp(StatEquip.m_Dmg * 1.2f);

	UMyLib::ReduceDurability(*m_CrntToolID, 1);
	
	if(m_nTreeHp<=0)
	{
		OnGatherDone();
	}
}

void ATreeBase::OnGatherDone()
{
	UMyLib::GetEquip()->AddItem(FItemSpec(m_DataRow->m_ItemGather.RowName,m_DataRow->m_nItemGatherCount));
	
	UMyGameInstance::Get->m_ZoneInst->RemoveFocusActor(this);
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),m_GatherAsset->m_SoundGatherEnd,GetActorLocation());
	
	m_MeshTree->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	m_ShadowMeshComp->SetHiddenInGame(true);
	
	if(m_bUsePhysics)
		m_MeshTree->SetSimulatePhysics(true);
}

void ATreeBase::OnHarvestMotionDone()
{
	m_Player->SetInteracting(false);
	
	m_CrntToolID = nullptr;
	
	m_Player->ShowWeapon();
	
	m_Player->UnbindCancel();
}

FVector ATreeBase::GetNavAgentLocation() const
{
	return GetActorLocation() - FVector(0.f, 0.f, 88);
}

void ATreeBase::CreateSetDeathCurve(float fullLength)
{
	m_CurveDeathAnim = FFloatCurve();
	
	m_CurveDeathAnim.UpdateOrAddKey(1, 0);
	
	m_CurveDeathAnim.UpdateOrAddKey(0, fullLength);
}

void ATreeBase::StartDeathEffectMaterial(float duration)
{
	m_fDeathAnimDurationMax = duration;

	m_fDeathAnimDurationTimer = 0.f;

	CreateSetDeathCurve(duration);
}

void ATreeBase::SetDeathEffectMaterial(float deltaTime)
{
	m_fDeathAnimDurationTimer+=deltaTime;

	float Value = m_CurveDeathAnim.Evaluate(m_fDeathAnimDurationTimer);

	FName MaskParam = TEXT("Visibility");

	m_MeshTree->SetScalarParameterValueOnMaterials(MaskParam, Value);
	
	m_MeshTrunk->SetScalarParameterValueOnMaterials(MaskParam, Value);
}

void ATreeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(m_nTreeHp<=0)
	{
		SetDeathEffectMaterial(DeltaSeconds);

		if(m_fDeathAnimDurationTimer>=m_fDeathAnimDurationMax)
		{
			SetActorTickEnabled(false);
			
			Destroy();
		}
	}
}

void ATreeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_GatherAsset.Reset();
}

void ATreeBase::GetMoveGoalReachTest(const AActor* MovingActor, const FVector& MoveOffset, FVector& GoalOffset,float& GoalRadius, float& GoalHalfHeight) const
{
	GoalOffset = FVector::ZeroVector;
	
	GoalRadius = GetNavAgentPropertiesRef().AgentRadius;

	GoalHalfHeight = GetNavAgentPropertiesRef().AgentHeight;
}

const FName& ATreeBase::GetID() const
{
	return m_ID;
}

int ATreeBase::GetHP()
{
	return m_nTreeHp;
}

FText ATreeBase::GetTextInteract()
{
	return NSLOCTEXT("ATreeBase","Gather","채집");
}

bool ATreeBase::IsInteractable()
{
	bool HasEquip = false;
	
	for(const FName& ToolID : m_GatherAsset->m_AryGatherToolID)
	{
		HasEquip = m_Player->GetAnyItemHave(ToolID) != nullptr;
		if(HasEquip)
		{
			break;
		}
	}
	
	bool InvenSpace = UMyLib::GetEquip()->HasSpace(FItemSpec(m_DataRow->m_ItemGather.RowName,m_DataRow->m_nItemGatherCount));
	
	return HasEquip && InvenSpace;
}
