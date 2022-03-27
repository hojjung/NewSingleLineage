#include "Skill_TurnBase.h"
#include "MyJrpg/Managers/MyAssetManager.h"

void USkill_TurnBase::EndSkillAnim()
{
	Super::EndSkillAnim();

	UUnitEntityAsset* LoadedAsset = UMyAssetManager::Get()->LoadUnitAsset(m_TurnSk.Get());

	m_Player->StopAnimMontage();
	
	m_Player->GetSkMesh()->SetAnimClass(nullptr);
	
	m_Player->GetSkMesh()->CompleteParallelAnimationEvaluation(false);

	m_Player->GetSkMesh()->SetSkeletalMesh(LoadedAsset->m_BodyMesh.Get());
	m_Player->GetSkMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
	m_Player->GetSkMesh()->SetAnimClass(LoadedAsset->m_AnimBP.Get());
}

void USkill_TurnBase::EndDuration()
{
	Super::EndDuration();
	
	m_Player->SetPlayerSkMeshDefault();
}
