#include "Skill_TurnDemon.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_TurnDemon::USkill_TurnDemon(const FObjectInitializer& obj)
{
	static ConstructorHelpers::FObjectFinder<UUnitEntityAsset> FoundAsset(TEXT("UnitEntityAsset'/Game/01_DataAssets/Units/PlayerUnit/Player02.Player02'"));
	m_AssetSoftLink = FoundAsset.Object;  
}
//ParticleSystem'/Game/04_VisualEffect/FantasyRPGVFXPack/Particles/PS_General_FireBig.PS_General_FireBig'
void USkill_TurnDemon::EndSkillAnim()
{
	Super::EndSkillAnim();
	m_Player->GetSkMesh()->SetRelativeScale3D(FVector(m_SkillData->m_fRange));
	m_Player->AddAtkRange(m_SkillData->m_fRange);
	GetPlayerStatus()->AddMultiAtkDmg(m_SkillData->m_fDamage);
	GetPlayerStatus()->SubMultiMoveSpeed(m_SkillData->m_fRadius);
}

void USkill_TurnDemon::EndDuration()
{
	Super::EndDuration();
	m_Player->GetSkMesh()->SetRelativeScale3D(FVector(0));
	m_Player->SubAtkRange(m_SkillData->m_fRange);
	GetPlayerStatus()->SubMultiAtkDmg(m_SkillData->m_fDamage);
	GetPlayerStatus()->AddMultiMoveSpeed(m_SkillData->m_fRadius);//slower
}

