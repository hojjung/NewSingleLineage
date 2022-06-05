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
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerPawn,const AMyPlayerPawn*);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuffChanged,UBuff_Base*);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTookDmg, float);

	DECLARE_MULTICAST_DELEGATE(FOnStatChanged);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnExpEarned, float);

public:
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

	FStatGroup m_BaseStatGroup;

	FStatGroup m_AddStatGroup;

	FStatGroup m_MultipleStatGroup;

protected://버프 아이템은 따로 뭔갈 안가질거임,남는시간?
	TMap<TSubclassOf<UBuff_Base>,float> m_MapBuffDur;
	
	TMap<TSubclassOf<UBuff_Base>,UBuff_Base*> m_MapBuffInst;
	UPROPERTY()
	TArray<UBuff_Base*> m_AryBuff;//for gc

	TSoftObjectPtr<UHumanAsset> m_BaseBodyWhite;

	TSoftObjectPtr<UHumanAsset> m_BaseBodyBlack;
	
public:
	void Init();
	
	void Tick(float deltaTime);

	void UpdateStat();
public://quest and special, delegates
	void OnMonsterKilled(AMonsterPawn* deadMonster);//플레이어가 몬스터 처치시

	void OnAttack(const AMyPlayerPawn* playerPawn);//플레이어가 공격시

	void OnPlHpChanged(const AMyPlayerPawn* playerPawn);//플레이어 피격시

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
};

