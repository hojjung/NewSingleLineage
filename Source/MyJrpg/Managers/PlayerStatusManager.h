// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/HumanAsset.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Items/Buff/Buff_Base.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "PlayerStatusManager.generated.h"

class AMyPlayerPawn;
/**
 * 플레이어 캐릭터 엔티티 스펙 관리
 * 모든 델리게이트도 이곳에?
 */
UCLASS()
class MYJRPG_API UPlayerStatusManager : public UObject
{
	GENERATED_BODY()

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerKilled,const ACombatUnitPawn*);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterPawn,AMonsterPawn*);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerPawn, const FStatGroup&);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuffChanged,UBuff_Base*);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTookDmg, float);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHungerTick, float);

	DECLARE_MULTICAST_DELEGATE(FOnStatChanged);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnExpEarned, float);

public:
	FOnHungerTick m_OnHungerChanged;
	
	FOnMonsterPawn m_OnMonsterKilled;

	FOnPlayerPawn m_OnPlayerAttack;

	FOnPlayerPawn m_OnPlayerHpChanged;

	FOnBuffChanged m_OnAddBuff;

	FOnBuffChanged m_OnRemoveBuff;

	FOnStatChanged m_OnExpChanged;
	
	FOnStatChanged m_OnLevelChanged;

	FOnStatChanged m_OnStatChanged;

	FOnPlayerKilled m_OnPlayerKilled;

	FOnTookDmg m_OnTookDmg;

	FOnExpEarned m_OnExpEarned;

protected:
	int m_nExp;

	int m_nMaxExp;

	int m_nLevel;

	float m_fHunger;

	float m_fDelayHunger;

	float m_fDamageHunger;

	float m_fHungerTimer;

	float m_fStarvDamageTimer;

	FStatGroup m_BaseStatGroup;

	FStatGroup m_AddStatGroup;

	FStatGroup m_MultipleStatGroup;

	FStatGroup m_CurrentStat;

protected://버프 아이템은 따로 뭔갈 안가질거임,남는시간?
	TMap<TSubclassOf<UBuff_Base>,float> m_MapBuffDur;
	
	TMap<TSubclassOf<UBuff_Base>,UBuff_Base*> m_MapBuffInst;
	UPROPERTY()
	TArray<UBuff_Base*> m_AryBuff;//for gc

	TSoftObjectPtr<UHumanAsset> m_BaseBodyWhite;

	TSoftObjectPtr<UHumanAsset> m_BaseBodyBlack;

protected:
	void HungerTick(float deltaTime);
	
	void TryTakeStarvDamage(float deltaTime);
	
	void BuffTick(float deltaTime);
	
public:
	void Init();

	void Tick(float deltaTime);

	void UpdateStat();

	void AddHunger(float am);

	void SubHunger(float am);
	
public://quest and special, delegates
	void OnMonsterKilled(AMonsterPawn* deadMonster);//플레이어가 몬스터 처치시

	void OnPlHpChanged(const FStatGroup& stat);//플레이어 피격시const FStatGroup& stat

	void OnPlTookDmg(float dmg);

public://buff
	void AddBuff(const FBuffDataRow*  class_buff);

	void RemoveBuff(TSubclassOf<UBuff_Base> class_buff);

	bool CheckBuffApplied(TSubclassOf<UBuff_Base> buffClass);
public://level and exp
	float GetMaxExp();

	float GetCExp();

	int GetLevel();

	void TryLevelUp();
	
	void AddExp(int amount);

public:
	void AddPlusDmgReduc(float v);

	void SubPlusDmgReduc(float v);
	
	void AddPlusCriPer(float v);

	void SubPlusCriPer(float v);
	
	void AddMultiMoveSpeed(float speedM);

	void SubMultiMoveSpeed(float speedM);

	void AddMultiAtkSpeed(float speedM);

	void SubMultiAtkSpeed(float speedM);

	void AddMultiAtkDmg(float v);
	
	void SubMultiAtkDmg(float v);

	const FStatGroup& GetStat() const;

	float GetHpPer() const;
	
	void SetBaseStat(const FStatGroup& stat_group);
	
	void EquipItem(const FName& item_spec);
	
	void UnequipItem(const FName& item_spec);

public:
	void OnPlayerDead(const ACombatUnitPawn* killer);
	
	const TSoftObjectPtr<UHumanAsset>& GetUnitAsset() const;

	float GetHungerHP();

	void ResetPlayerStatus();
	
	void SubDmgFromHp(float dmg);
	
	void AddHp(float dmg);
};

