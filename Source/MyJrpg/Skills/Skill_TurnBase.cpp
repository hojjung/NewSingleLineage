#include "Skill_TurnBase.h"
#include "MyJrpg/Managers/MyAssetManager.h"


void USkill_TurnBase::EndSkillAnim()
{
	Super::EndSkillAnim();

	UMyAssetManager::Get()->LoadUnitAsset(m_TurnSk,FStreamableDelegate::CreateUObject(this, &USkill_TurnBase::OnLoadComplete,m_TurnSk));

	
}

void USkill_TurnBase::OnLoadComplete(UUnitEntityAsset* asset)
{
	m_Player->StopAnimMontage();
	
	m_Player->GetSkMesh()->SetAnimClass(nullptr);
	
	m_Player->GetSkMesh()->CompleteParallelAnimationEvaluation(false);

	m_Player->GetSkMesh()->SetSkeletalMesh(asset->m_BodyMesh);
	m_Player->GetSkMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
	m_Player->GetSkMesh()->SetAnimClass(asset->m_AnimBP.Get());
}

void USkill_TurnBase::EndDuration()
{
	Super::EndDuration();
	
	m_Player->SetPlayerSkMeshDefault();
}
