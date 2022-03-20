#include "Skill_LeapAttack.h"
#include "NavigationSystem.h"
#include "TweenUtil.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Pawns/Components/MyMovement.h"

void USkill_LeapAttack::UseSkill()
{
	Super::UseSkill();
}

void USkill_LeapAttack::EndSkillAnim()
{
	Super::EndSkillAnim();
}

bool USkill_LeapAttack::CanUseSkill()
{
	if(!m_Player)
	{
		m_Player = UMyLib::GetPlayer();
	}
	
	if (CheckCooltime())
	{
		FVector StartPoint = m_Player->GetActorLocation();

		m_EndPoint = (m_Player->GetActorForwardVector() * m_SkillData->m_fRange) + StartPoint;

		FNavLocation NavLoc;

		if(!UMyLib::GetNavSys()->ProjectPointToNavigation(m_EndPoint,NavLoc))
		{
			return false;
		}
		
		m_EndPoint = NavLoc.Location;

		if(!GetWorld()->FindTeleportSpot(m_Player,m_EndPoint,m_Player->GetActorRotation()))
		{
			return false;
		}
	
		return true;
	}

	return false;
}

void USkill_LeapAttack::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	PRINTF("USkill_LeapAttack::OnSkillTriggered");

	FVector StartPoint = m_Player->GetActorLocation();

	m_Player->ClearStopMoveDelegate();
	
	m_Player->StopMove();
	
	m_Player->GetMovementComponent()->SetActive(false);

	UTweenPath* PathTween = UTweenUtil::DOPathJumpSync(StartPoint, m_EndPoint,500,m_SkillData->m_fDuration);
	
	PathTween->OnUpdate.AddDynamic(this,&USkill_LeapAttack::OnUpdateMove);
	
	PathTween->OnComplete.AddDynamic(this,&USkill_LeapAttack::OnComplete);
}

void USkill_LeapAttack::OnUpdateMove(FVector curPos, UTweenBase* tween)
{
	m_Player->SetActorLocation(curPos);
}

void USkill_LeapAttack::OnComplete(FVector curPos, UTweenBase* tween)
{
	m_MoveTween = nullptr;
	
	m_Player->GetMovementComponent()->SetActive(true);
	
	FVector PlayerPoint = m_Player->GetMovementComponent()->GetActorFeetLocation();

	TArray<AActor*> Hits;
	
	if(!TraceSphere(Hits, PlayerPoint, m_SkillData->m_fRadius))
	{
		return;
	}
	
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		
		Monster->TakeDmg(GetDamage(), m_Player);
	}
}