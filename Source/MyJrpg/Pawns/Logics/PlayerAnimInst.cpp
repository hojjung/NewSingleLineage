#include "PlayerAnimInst.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UPlayerAnimInst::Init(const FPlayerUnitEntityRow& row, AMyPlayerPawn* pl)
{
	m_EntityRow = &row;
	
	m_Owner = pl;
	
	m_AryHandle.Reset();
	m_AryAnims.Reset();
	m_MapSkills.Reset();

	for(const FPlayerSkillAnim& AnimData : row.m_ArySkillAnims)
	{
		TSharedPtr<FStreamableHandle> Handle = UMyAssetManager::Get()->LoadAnimMontage(AnimData.m_SkillMotion);

		m_AryHandle.Add(Handle);
		m_AryAnims.Add(AnimData.m_SkillMotion.Get());
		m_MapSkills.Add(AnimData.m_SkillID,AnimData.m_SkillMotion.Get());
	}
	
	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UPlayerAnimInst::OnEquipChanged);
	
	OnEquipChanged();
}

void UPlayerAnimInst::OnEquipChanged()
{
	if(UMyGameInstance::Get->m_EquipManager->IsRangeStance())
	{
		SetStanceMage();
	}
	else
	{
		SetStanceSword();
	}
}

void UPlayerAnimInst::SetStanceSword()
{
	m_Owner->StopAnimMontage();

	if (m_BaseAttackHandle.IsValid())
		m_BaseAttackHandle->ReleaseHandle();

	m_BaseAttackHandle = UMyAssetManager::Get()->LoadAnimMontage(m_EntityRow->m_StanceSword);
	
	m_BaseAttack = m_EntityRow->m_StanceSword.Get();
}

void UPlayerAnimInst::SetStanceMage()
{
	m_Owner->StopAnimMontage();

	if (m_BaseAttackHandle.IsValid())
		m_BaseAttackHandle->ReleaseHandle();

	m_BaseAttackHandle = UMyAssetManager::Get()->LoadAnimMontage(m_EntityRow->m_StanceMage);
	
	m_BaseAttack = m_EntityRow->m_StanceMage.Get();
	//
	auto* Effect =  UMyGameInstance::Get->m_EquipManager->GetBulletEffect();
	
	m_Owner->CreateRangeBullet(Effect);
}

UAnimMontage* UPlayerAnimInst::GetSkillAnim(FName skillID)
{
	return m_MapSkills[skillID];
}

void UPlayerAnimInst::KillAll()
{
	for(auto& Iter : m_AryHandle)
	{
		Iter.Get()->ReleaseHandle();
	}

	m_AryHandle.Reset();
	
	CollectGarbage(EObjectFlags::RF_Public);
}

UAnimMontage* UPlayerAnimInst::GetBasAttackAnim()
{
	return m_BaseAttack;
}
