#include "SkillBase.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Pawns/Components/MyMovement.h"

void USkillBase::SetSkill(FName id,const FSkillDataRow& dataRow, int skillLevel)
{
	m_fCooltime = 0;
	m_Id = id;
	m_SkillData = &dataRow;
	m_nSkillLevel = skillLevel;
}

void USkillBase::UseSkill()
{
	PRINTF("UseSkill:%s",*m_Id.ToString());

	m_fCooltime = GetMaxCooltime();
	m_Player->ClearStopMoveDelegate();
	m_SkillHandle = m_Player->GetSkillTriggerDelegate().AddUObject(this,&USkillBase::CheckSkillTriggered);
	m_Player->StopAnimMontage();
	m_Player->SetSkillUsing(true);
	m_Player->SetCanMoveInSkill(m_SkillData->m_bCanMove);
	m_Player->StopMove();
	PlayAnimMontage();
}

void USkillBase::EndSkillAnim()
{
	PRINTF("EndSkill:%s",*m_Id.ToString());
	m_Player->SetSkillUsing(false);
	m_Player->SetCanMoveInSkill(false);
	m_Player->GetSkillTriggerDelegate().Remove(m_SkillHandle);
	GetWorld()->GetTimerManager().ClearTimer(m_SkillEnd);
}

bool USkillBase::CanUseSkill()
{
	if(!m_Player)
	{
		m_Player =  UMyLib::GetPlayer();	
	}
	return CheckCooltime() && IsTargetInRange();
}

void USkillBase::Tick(float delta_time)
{
	m_fCooltime -= delta_time;
}

void USkillBase::CheckSkillTriggered(FName id)
{
	if (m_Id != id)
	{
		return ;
	}
	
	OnSkillTriggered();
}

void USkillBase::OnSkillTriggered()
{
	
}

void USkillBase::PlayAnimMontage()
{
	float MonLen = m_Player->PlaySkillAnim(m_Id);
	
	GetWorld()->GetTimerManager().SetTimer(m_SkillEnd, this, &USkillBase::EndSkillAnim, MonLen, false);
}

bool USkillBase::TraceBox(TArray<AActor*>& outHits, float range, FVector boxHalfSize, TSubclassOf<AMonsterPawn> classFilter)
{
	AMyPlayerPawn* Instigator = m_Player;
	
	FVector StartTrace = Instigator->GetActorLocation() +(Instigator->GetActorForwardVector() * range);

	DrawDebugBox(GetWorld(),StartTrace,boxHalfSize,Instigator->GetActorRotation().Quaternion(),FColor::Red,false,1);
	if(!UMyLib::BoxOverlapActors(Instigator,Instigator->GetActorRotation(),StartTrace,boxHalfSize,
		Instigator->GetTraceObjTypes(),classFilter,Instigator->GetTraceIgnoredActors(),outHits))
	{
		return false;
	}
	return true;
}

bool USkillBase::TraceSphere(TArray<AActor*>& outHits, float range, float radius, TSubclassOf<AMonsterPawn> classFilter)
{
	AMyPlayerPawn* Instigator = m_Player;
	
	FVector StartTrace = Instigator->GetActorLocation() +(Instigator->GetActorForwardVector() * range);
	
	return TraceSphere(outHits, StartTrace, radius, classFilter);
}

bool USkillBase::TraceSphere(TArray<AActor*>& outHits, FVector start, float radius,
	TSubclassOf<AMonsterPawn> classFilter)
{
	AMyPlayerPawn* Instigator = m_Player;

	DrawDebugSphere(GetWorld(),start,radius,12,FColor::Red,false,1);
	if(!UMyLib::SphereOverlapActors(Instigator,Instigator->GetActorRotation(),start,radius,
		Instigator->GetTraceObjTypes(),classFilter,Instigator->GetTraceIgnoredActors(),outHits))
	{
		return false;
	}
	return true;
}

UPlayerStatusManager* USkillBase::GetPlayerStatus()
{
	return UMyGameInstance::Get->m_PlayerStatManager;
}

bool USkillBase::IsTargetInRange()
{
	if(!m_Player->GetFocusedTarget())
	{
		return false;
	}

	float RangeSqr = m_SkillData->m_fRange * m_SkillData->m_fRange;

	float LengthSqr = FVector::DistSquared2D(m_Player->GetActorLocation(),m_Player->GetFocusedTarget()->GetActorLocation());

	return LengthSqr <= RangeSqr;
}

float USkillBase::GetDamage()
{
	return UMyGameInstance::Get->m_SkillManager->GetSkillDmg(m_Id); 
}

const FSkillDataRow& USkillBase::GetSkillData() const
{
	return *m_SkillData;
}

bool USkillBase::CheckCooltime() const
{
	return m_fCooltime <= 0;
}

float USkillBase::GetCrnCooltime() const
{
	return m_fCooltime; 
}

float USkillBase::GetMaxCooltime() const
{
	return UMyGameInstance::Get->m_SkillManager->GetSkillCD(m_Id);
}