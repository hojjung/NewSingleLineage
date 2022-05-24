#include "PlayerStatusManager.h"

#include "MyAssetManager.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"

void UPlayerStatusManager::Init()
{
	//m_BaseStatGroup
	m_nExp = 0;

	m_nMaxExp = 550;

	m_nLevel = 1;

	m_BaseStatGroup = FStatGroup();

	m_AddStatGroup = FStatGroup(0);

	m_MultipleStatGroup = FStatGroup(1);

	m_BaseBodyWhite = TSoftObjectPtr<UHumanAsset>(FSoftObjectPath(TEXT("HumanAsset'/Game/01_DataAssets/Humans/DefaultWhite.DefaultWhite'")));
	
	m_BaseBodyBlack = TSoftObjectPtr<UHumanAsset>(FSoftObjectPath(TEXT("HumanAsset'/Game/01_DataAssets/Humans/DefaultBlack.DefaultBlack'")));

	UpdateStat();
}

void UPlayerStatusManager::Tick(float deltaTime)
{
	for (int32 i = m_AryBuff.Num() - 1; i >= 0; --i)
	{
		USkill_BuffBase* Buff = m_AryBuff[i];

		float RemainTime = m_MapBuffDur[Buff->GetClass()];

		RemainTime -= deltaTime;

		Buff->OnTick(deltaTime);

		if (RemainTime <= 0)
		{
			Buff->EndDuration();

			RemoveBuff(m_AryBuff[i]);
		}
		else
		{
			m_MapBuffDur[Buff->GetClass()] = RemainTime;
		}
	}
}

void UPlayerStatusManager::UpdateStat()
{
	FStatGroup ResultStatGroup = (m_BaseStatGroup + m_AddStatGroup) * m_MultipleStatGroup;

	AMyPlayerPawn* Pl = UMyLib::GetPlayer();

	if (Pl)
	{
		Pl->UpdateStat(ResultStatGroup);
	}

	m_OnStatChanged.Broadcast();
}

void UPlayerStatusManager::OnMonsterKilled(AMonsterPawn* deadMonster)
{
	m_OnMonsterKilled.Broadcast(deadMonster);
}

void UPlayerStatusManager::OnAttack(const AMyPlayerPawn* playerPawn)
{
	m_OnPlayerAttack.Broadcast(playerPawn);
}

void UPlayerStatusManager::OnPlHpChanged(const AMyPlayerPawn* playerPawn)
{
	m_OnPlayerHpChanged.Broadcast(playerPawn);
}

void UPlayerStatusManager::OnPlTookDmg(float dmg)
{
	m_OnTookDmg.Broadcast(dmg);
}

void UPlayerStatusManager::AddBuff(USkill_BuffBase* buff)
{
	if (m_MapBuffInst.Find(buff->GetClass()))
	{
		RemoveBuff(m_MapBuffInst[buff->GetClass()]);
	}

	m_AryBuff.Add(buff);

	m_MapBuffDur.Add(buff->GetClass(), buff->GetDuration());

	m_MapBuffInst.Add(buff->GetClass(), buff);

	m_OnAddBuff.Broadcast(buff);
}

void UPlayerStatusManager::RemoveBuff(USkill_BuffBase* buff)
{
	m_MapBuffDur.Remove(buff->GetClass());

	m_MapBuffInst.Remove(buff->GetClass());

	m_AryBuff.Remove(buff);

	m_OnRemoveBuff.Broadcast(buff);
}

bool UPlayerStatusManager::CheckBuffApplied(TSubclassOf<USkill_BuffBase> buffClass)
{
	return m_MapBuffDur.Contains(buffClass);
}

float UPlayerStatusManager::GetMaxExp()
{
	return m_nMaxExp;
}

float UPlayerStatusManager::GetCExp()
{
	return m_nExp;
}

int UPlayerStatusManager::GetLevel()
{
	return m_nLevel;
}

void UPlayerStatusManager::TryLevelUp()
{
	if (m_nLevel >= FGlobalVariable::LEVEL_MAX)
	{
		return;
	}
	m_nLevel++;

	m_nExp = FMath::Max(m_nExp - m_nMaxExp, 0);

	m_nMaxExp = m_nMaxExp * 1.2f;

	m_OnLevelChanged.Broadcast();
}

void UPlayerStatusManager::AddExp(int amount)
{
	if (m_nLevel >= FGlobalVariable::LEVEL_MAX)
	{
		return;
	}

	m_nExp += amount;

	if (m_nExp >= GetMaxExp())
	{
		TryLevelUp();
	}

	m_OnExpChanged.Broadcast();
}

void UPlayerStatusManager::AddPlusDmgReduc(float v)
{
	m_AddStatGroup.m_DmgReduce += v;
	UpdateStat();
}

void UPlayerStatusManager::SubPlusDmgReduc(float v)
{
	m_AddStatGroup.m_DmgReduce -= v;
	UpdateStat();
}

void UPlayerStatusManager::AddPlusCriPer(float v)
{
	m_AddStatGroup.m_CriPer += v;
	UpdateStat();
}

void UPlayerStatusManager::SubPlusCriPer(float v)
{
	m_AddStatGroup.m_CriPer -= v;
	UpdateStat();
}

void UPlayerStatusManager::AddMultiMoveSpeed(float speedM)
{
	m_MultipleStatGroup.m_MoveSpeed *= speedM;
	UpdateStat();
}

void UPlayerStatusManager::SubMultiMoveSpeed(float speedM)
{
	m_MultipleStatGroup.m_MoveSpeed /= speedM;
	UpdateStat();
}

void UPlayerStatusManager::AddMultiAtkSpeed(float speedM)
{
	m_MultipleStatGroup.m_AtkSpeed *= speedM;
	UpdateStat();
}

void UPlayerStatusManager::SubMultiAtkSpeed(float speedM)
{
	m_MultipleStatGroup.m_AtkSpeed /= speedM;
	UpdateStat();
}

void UPlayerStatusManager::AddMultiAtkDmg(float v)
{
	m_MultipleStatGroup.m_Dmg *= v;
	UpdateStat();
}

void UPlayerStatusManager::SubMultiAtkDmg(float v)
{
	m_MultipleStatGroup.m_Dmg /= v;
	UpdateStat();
}

const FStatGroup& UPlayerStatusManager::GetStat() const
{
	return UMyLib::GetPlayer()->GetStat();
}

float UPlayerStatusManager::GetHpPer() const
{
	const FStatGroup& Stat = GetStat();

	return Stat.m_Hp / Stat.m_MaxHp;
}

void UPlayerStatusManager::SetBaseStat(const FStatGroup& stat_group)
{
	m_BaseStatGroup = stat_group;
}

void UPlayerStatusManager::EquipItem(const FName& item_spec)
{
	m_AddStatGroup += UMyLib::GetItemStatData(item_spec);
	UpdateStat();
}

void UPlayerStatusManager::UnequipItem(const FName& item_spec)
{
	m_AddStatGroup -= UMyLib::GetItemStatData(item_spec);
	UpdateStat();
}

void UPlayerStatusManager::OnPlayerDead(const ACombatUnitPawn* killer)
{
	m_OnPlayerKilled.Broadcast(killer);
	
}

const TSoftObjectPtr<UHumanAsset>& UPlayerStatusManager::GetUnitAsset() const
{
	return m_BaseBodyWhite;
}
