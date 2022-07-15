#include "PlayerStatusManager.h"

#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/BuffTable.h"
#include "MyJrpg/Items/Buff/Buff_Base.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"

void UPlayerStatusManager::Init()
{
	//m_BaseStatGroup
	m_fStarvDamageTimer = 0;
	
	m_fHungerTimer = 0;
	
	m_fHunger = 100;

	m_fDelayHunger = FGlobalVariable::HUNGER_DELAY;

	m_fDamageHunger = FGlobalVariable::HUNGER_DAMAGE;
	
	m_nExp = 0;

	m_nMaxExp = 10;

	m_nLevel = 1;

	m_BaseStatGroup = FStatGroup();

	m_AddStatGroup = FStatGroup(0);

	m_MultipleStatGroup = FStatGroup(1);

	m_BaseBodyWhite = TSoftObjectPtr<UHumanAsset>(FSoftObjectPath(TEXT("HumanAsset'/Game/01_DataAssets/Humans/DefaultWhite.DefaultWhite'")));
	
	m_BaseBodyBlack = TSoftObjectPtr<UHumanAsset>(FSoftObjectPath(TEXT("HumanAsset'/Game/01_DataAssets/Humans/DefaultBlack.DefaultBlack'")));

	FStatGroup DefaultStat;
	DefaultStat.m_nAccu = 10;
	DefaultStat.m_nAvoid = 0;
	DefaultStat.m_MaxHp = 100;
	DefaultStat.m_Dmg = 2;
	DefaultStat.m_AtkPerSec = 0.8f;
	DefaultStat.m_DmgReduce = 0;
	DefaultStat.m_CriPer = 0.0f;
	DefaultStat.m_CriDmg = 1.5f;
	DefaultStat.m_MoveSpeed = FGlobalVariable::HERO_DEFAULT_SPEED;
	
	SetBaseStat(DefaultStat);
	
	UpdateStat();
}

void UPlayerStatusManager::HungerTick(float deltaTime)
{
	m_fHungerTimer += deltaTime;

	if(m_fHungerTimer >= m_fDelayHunger)
	{
		m_fHungerTimer = 0;

		SubHunger(m_fDamageHunger);
	}
}

void UPlayerStatusManager::TryTakeStarvDamage(float deltaTime)
{
	if(m_fHunger <= 0)
	{
		m_fStarvDamageTimer += deltaTime;

		if(m_fStarvDamageTimer > 1)
		{
			float Dmg = GetStat().m_MaxHp * 0.04f;

			UMyLib::GetPlayer()->TakeDmg(Dmg, nullptr);
			
			m_fStarvDamageTimer = 0;
		}
	}
}

void UPlayerStatusManager::BuffTick(float deltaTime)
{
	for (int32 i = m_AryBuff.Num() - 1; i >= 0; --i)
	{
		UBuff_Base* Buff = m_AryBuff[i];

		float RemainTime = m_MapBuffDur[Buff->GetClass()];

		RemainTime -= deltaTime;

		Buff->OnTick(deltaTime);

		if (RemainTime <= 0)
		{
			Buff->EndBuff();

			RemoveBuff(m_AryBuff[i]->GetClass());
		}
		else
		{
			m_MapBuffDur[Buff->GetClass()] = RemainTime;
		}
	}
}

void UPlayerStatusManager::Tick(float deltaTime)
{
	HungerTick(deltaTime);

	TryTakeStarvDamage(deltaTime);
	
	BuffTick(deltaTime);
}

void UPlayerStatusManager::UpdateStat()
{
	float CHp = m_CurrentStat.m_Hp;
	
	m_CurrentStat = (m_BaseStatGroup + m_AddStatGroup) * m_MultipleStatGroup;

	if(m_CurrentStat.m_MaxHp < CHp)
	{
		m_CurrentStat.m_Hp = m_CurrentStat.m_MaxHp;
	}
	else
	{
		m_CurrentStat.m_Hp = CHp;
	}
	m_OnPlayerHpChanged.Broadcast(m_CurrentStat);

	AMyPlayerPawn* Pl = UMyLib::GetPlayer();

	if (Pl)
	{
		Pl->UpdateStat(m_CurrentStat);
	}

	m_OnStatChanged.Broadcast();
}

void UPlayerStatusManager::AddHunger(float am)
{
	m_fHunger += am;

	m_fHunger = FMath::Min(m_fHunger, 100.f);
	
	m_OnHungerChanged.Broadcast(m_fHunger);
}

void UPlayerStatusManager::SubHunger(float am)
{
	m_fHunger -= am;

	m_fHunger = FMath::Max(m_fHunger, 0.f);
	
	m_OnHungerChanged.Broadcast(m_fHunger);
}

void UPlayerStatusManager::OnMonsterKilled(AMonsterPawn* deadMonster)
{
	m_OnMonsterKilled.Broadcast(deadMonster);
}

void UPlayerStatusManager::OnPlHpChanged(const FStatGroup& stat)
{
	m_OnPlayerHpChanged.Broadcast(stat);
}

void UPlayerStatusManager::OnPlTookDmg(float dmg)
{
	m_OnTookDmg.Broadcast(dmg);
}

void UPlayerStatusManager::AddBuff(const FBuffDataRow*  data)
{
	if (m_MapBuffInst.Find(data->m_ClassBuff))
	{
		RemoveBuff(data->m_ClassBuff);
	}

	UBuff_Base* BuffInst = NewObject<UBuff_Base>(this,data->m_ClassBuff);

	BuffInst->Init(*data);
	
	m_AryBuff.Emplace(BuffInst);

	m_MapBuffDur.Add(data->m_ClassBuff, BuffInst->GetDuration());

	m_MapBuffInst.Add(data->m_ClassBuff, BuffInst);

	m_OnAddBuff.Broadcast(BuffInst);
}

void UPlayerStatusManager::RemoveBuff(TSubclassOf<UBuff_Base> class_buff)
{
	m_MapBuffDur.Remove(class_buff->GetClass());

	UBuff_Base** BuffInst = m_MapBuffInst.Find(class_buff);

	m_MapBuffInst.Remove(class_buff);

	m_AryBuff.Remove(*BuffInst);

	m_OnRemoveBuff.Broadcast(*BuffInst);
}

bool UPlayerStatusManager::CheckBuffApplied(TSubclassOf<UBuff_Base> buffClass)
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

	m_nExp = m_nExp - m_nMaxExp;//150 140
	
	m_nMaxExp = m_nMaxExp * 1.2f;

	m_OnLevelChanged.Broadcast();
}

void UPlayerStatusManager::AddExp(int amount)
{
	if (m_nLevel >= FGlobalVariable::LEVEL_MAX)
	{
		return;
	}

	m_OnExpEarned.Broadcast(amount);
	
	m_nExp += amount;

	while (m_nLevel < FGlobalVariable::LEVEL_MAX && m_nExp >= GetMaxExp())
	{
		m_nLevel++;
		
		m_nExp -= m_nMaxExp;
		m_nMaxExp = m_nMaxExp * 1.2f;
		
		m_OnLevelChanged.Broadcast();
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
	m_MultipleStatGroup.m_AtkPerSec *= speedM;
	UpdateStat();
}

void UPlayerStatusManager::SubMultiAtkSpeed(float speedM)
{
	m_MultipleStatGroup.m_AtkPerSec /= speedM;
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
	return m_CurrentStat;
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

float UPlayerStatusManager::GetHungerHP()
{
	return m_fHunger;
}

void UPlayerStatusManager::ResetPlayerStatus()
{
	m_fHunger = 100;

	UpdateStat();
	
	m_CurrentStat.m_Hp = m_CurrentStat.m_MaxHp;

	UMyGameInstance::Get->m_ZoneInst->ResetZone();
}

void UPlayerStatusManager::SubDmgFromHp(float dmg)
{
	m_CurrentStat.m_Hp -= dmg;

	m_CurrentStat.m_Hp = FMath::Max(GetStat().m_Hp, 0.f);

	m_OnPlayerHpChanged.Broadcast(GetStat());
}

void UPlayerStatusManager::AddHp(float dmg)
{
	m_CurrentStat.m_Hp += dmg;

	m_CurrentStat.m_Hp = FMath::Min(GetStat().m_Hp,GetStat().m_MaxHp);

	m_OnPlayerHpChanged.Broadcast(GetStat());
}
