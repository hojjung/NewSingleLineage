#include "MobSkillBase.h"

#include "MyJrpg/MyLib.h"

UMobSkillBase::UMobSkillBase()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Mat(TEXT("Material'/Game/03_VisualEffect/Decals/M_EnemySkillIndicator.M_EnemySkillIndicator'"));

	m_Mat = Mat.Object;

	//m_SkillMotion

	//m_Id
}

void UMobSkillBase::Init(ACombatUnitPawn* owner)
{
	m_Owner = owner;

	m_bIsSkillUsing = false;
}

void UMobSkillBase::SpawnDecal()
{//1:200,2:400
	m_Decal = m_Owner->GetWorld()->SpawnActor<ADecalActor>(ADecalActor::StaticClass(),m_Owner->GetActorLocation(),m_Owner->GetActorRotation());
	m_Decal->SetDecalMaterial(m_Mat);
	m_Decal->SetActorRelativeScale3D(FVector(m_fRadius / 200.f));
}

void UMobSkillBase::PlayAnimMontage()
{
	float MonLen = m_Owner->PlayAnimMontage(m_SkillMotion);
	
	m_Owner->GetWorld()->GetTimerManager().SetTimer(m_SkillEnd, this, &UMobSkillBase::EndSkillAnim, MonLen, false);
}

bool UMobSkillBase::TraceBox(TArray<AActor*>& outHits, float range, FVector boxHalfSize,
	TSubclassOf<ACombatUnitPawn> classFilter)
{
	FVector StartTrace = m_Owner->GetActorLocation() +(m_Owner->GetActorForwardVector() * range);
	DrawDebugBox(GetWorld(),StartTrace,boxHalfSize,m_Owner->GetActorRotation().Quaternion(),FColor::Red,false,1);
	if(!UMyLib::BoxOverlapActors(m_Owner,m_Owner->GetActorRotation(),StartTrace,boxHalfSize,
		m_Owner->GetTraceObjTypes(),classFilter,m_Owner->GetTraceIgnoredActors(),outHits))
	{
		return false;
	}
	return true;
}

bool UMobSkillBase::TraceSphere(TArray<AActor*>& outHits, float range, float radius,
	TSubclassOf<ACombatUnitPawn> classFilter)
{
	FVector StartTrace = m_Owner->GetActorLocation() +(m_Owner->GetActorForwardVector() * range);
	return TraceSphere(outHits, StartTrace, radius, classFilter);
}

bool UMobSkillBase::TraceSphere(TArray<AActor*>& outHits, FVector start, float radius,
	TSubclassOf<ACombatUnitPawn> classFilter)
{
	DrawDebugSphere(GetWorld(),start,radius,12,FColor::Red,false,1);
	if(!UMyLib::SphereOverlapActors(m_Owner,m_Owner->GetActorRotation(),start,radius,
		m_Owner->GetTraceObjTypes(),classFilter,m_Owner->GetTraceIgnoredActors(),outHits))
	{
		return false;
	}
	return true;
}

void UMobSkillBase::UseSkill()
{
	m_bIsSkillUsing = true;
	m_SkillHandle = m_Owner->GetSkillTriggerDelegate().AddUObject(this,&UMobSkillBase::CheckSkillTriggered);
	m_Owner->StopAnimMontage();
	m_Owner->StopMove();
	PlayAnimMontage();
}

void UMobSkillBase::OnTriggerSkill()
{
	
}

void UMobSkillBase::CheckSkillTriggered(FName id)
{
	if (m_Id != id)
	{
		return ;
	}
	
	OnTriggerSkill();
}

bool UMobSkillBase::IsUsing()
{
	return m_bIsSkillUsing;
}
void UMobSkillBase::KillDecal()
{
	if(m_Decal)
		m_Decal->Destroy();
}

void UMobSkillBase::EndSkillAnim()
{
	m_bIsSkillUsing = false;
	m_Owner->GetSkillTriggerDelegate().Remove(m_SkillHandle);
	GetWorld()->GetTimerManager().ClearTimer(m_SkillEnd);

	KillDecal();
}