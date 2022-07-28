#include "SkillManager.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USkillManager::Init()
{
	m_MapSkillSpecs.Reset();

	m_MapSkillInsts.Reset();

	//m_AryEquipSkills.Init(NAME_None, FGlobalVariable::QUICKSLOT_MAX);

	//m_AryEquipSkillInst.Init(nullptr, FGlobalVariable::QUICKSLOT_MAX);

	TArray<FName> ArySkillNames = USkillData::GetSkillTable->GetRowNames();

	TArray<FSkillDataRow*> ArySkill;
	
	USkillData::GetSkillTable->GetAllRows<FSkillDataRow>("",ArySkill);

	int i = 0;

	while (i < ArySkill.Num())
	{
		FName SkillID = ArySkillNames[i];
		
		m_MapSkillSpecs.Add(SkillID,true);
		
		i++;
	}
}

bool USkillManager::ExecuteSkill(int index)
{
	const FName& SkillID = m_AryEquipSkills[index];
	
	if(!m_MapSkillInsts[SkillID]->CanUseSkill())
	{
		return false;
	}

	m_MapSkillInsts[SkillID]->UseSkill();

	m_OnSkillUse.Broadcast(index);

	return true;
}

bool USkillManager::ExecuteSkill(USkillBase* skill)
{
	return ExecuteSkill(m_AryEquipSkillInst.Find(skill));
}

const FSkillDataRow* USkillManager::GetSkillData(int index)
{
	if (m_AryEquipSkills[index] == NAME_None)
	{
		return nullptr;
	}
	return USkillData::GetSkillTable->FindRow<FSkillDataRow>(m_AryEquipSkills[index],""); 
}

const FSkillDataRow& USkillManager::GetSkillData(const FName& name)
{
	return *USkillData::GetSkillTable->FindRow<FSkillDataRow>(name,"");
}

void USkillManager::Tick(float deltaTime)
{
	for(auto* SkillInst : m_AryEquipSkillInst)
	{
		if(!SkillInst)
		{
			continue;
		}
		SkillInst->Tick(deltaTime);
	}
}

void USkillManager::EquipSkill(int index, const FName& name)//쿨타임중인 스킬 해제될수있나? 사용중인 스킬 해제될수있나?
{
	if(UMyLib::GetPlayer()->GetSkillUsing())
	{
		return;
	}
	int FoundIndex = m_AryEquipSkills.Find(name);

	if (FoundIndex != INDEX_NONE)
	{
		if (!m_MapSkillInsts[m_AryEquipSkills[FoundIndex]]->CheckCooltime())
		{
			return;
		}
		UnequipSkill(FoundIndex);
	}
	//
	m_AryEquipSkills[index] = name;

	const FSkillDataRow* SkillData = USkillData::GetSkillTable->FindRow<FSkillDataRow>(name,"");

	USkillBase* SkillInst = NewObject<USkillBase>(this, SkillData->m_ClassSkill);

	SkillInst->SetSkill(name, *SkillData, m_MapSkillSpecs[name]);
	
	m_AryEquipSkillInst[index] = SkillInst;
	
	m_MapSkillInsts.Add(name, SkillInst);

	m_OnSkillChanged.Broadcast();
}

void USkillManager::UnequipSkill(int index)
{
	m_MapSkillInsts.Remove(m_AryEquipSkills[index]);
	
	m_AryEquipSkillInst[index] = nullptr;
	
	m_AryEquipSkills[index] = NAME_None;

	m_OnSkillChanged.Broadcast();
}

const TMap<FName, bool>& USkillManager::GetSkillUnlocks() const
{
	return m_MapSkillSpecs;
}

float USkillManager::GetSkillCD(const FName& name)
{
	float cd = GetSkillData(name).m_fSkillCooldown;

	return cd;
}

float USkillManager::GetSkillCD(int index)
{
	return GetSkillCD(m_AryEquipSkills[index]);
}

float USkillManager::GetSkillDmg(const FName& name)
{
	float dmg = GetSkillData(name).m_fDamage;

	return dmg;
}

const TArray<FName>& USkillManager::GetEquipSkillID() const
{
	return m_AryEquipSkills;
}

const TArray<USkillBase*>& USkillManager::GetEquipSkillInstAry() const
{
	return m_AryEquipSkillInst;
}
