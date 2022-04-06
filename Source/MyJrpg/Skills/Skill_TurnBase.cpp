#include "Skill_TurnBase.h"
#include "MyJrpg/Managers/MyAssetManager.h"


void USkill_TurnBase::EndSkillAnim()
{
	Super::EndSkillAnim();

	m_LoadedAssest = UMyAssetManager::Get()->LoadUnitAsset(m_AssetSoftLink);

	m_Player->StopAnimMontage();
	
	m_Player->GetSkMesh()->SetAnimClass(nullptr);
	
	m_Player->GetSkMesh()->CompleteParallelAnimationEvaluation(false);

	m_Player->GetSkMesh()->SetSkeletalMesh(m_LoadedAssest->m_BodyMesh);
	m_Player->GetSkMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
	m_Player->GetSkMesh()->SetAnimClass(m_LoadedAssest->m_AnimBP);
}

void USkill_TurnBase::EndDuration()
{
	Super::EndDuration();
	
	m_Player->SetPlayerSkMeshDefault();

	m_LoadedAssest = nullptr;
}
