// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/SkillData.h"

#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "UObject/NoExportTypes.h"
#include "SkillManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkillManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnSkillChanged);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillUse, int);

	FOnSkillChanged m_OnSkillChanged;

	FOnSkillUse m_OnSkillUse;
	
protected:
	TMap<FName,bool> m_MapSkillSpecs;//스킬의 레벨 저장
	
	TMap<FName,USkillBase*> m_MapSkillInsts;//인스턴스
	UPROPERTY()
	TArray<FName> m_AryEquipSkills;//퀵슬롯,저장
	UPROPERTY()//gc
	TArray<USkillBase*> m_AryEquipSkillInst;
	
public:
	void Init();

	bool ExecuteSkill(int index);

	bool ExecuteSkill(USkillBase* skill);
	
	const FSkillDataRow * GetSkillData(int index);

	const FSkillDataRow & GetSkillData(const FName& name);

	void Tick(float deltaTime);

	const TArray<FName>& GetEquipSkillID() const;

	const TArray<USkillBase*>& GetEquipSkillInstAry() const;

	void EquipSkill(int index, const FName& name);

	void UnequipSkill(int index);
	
	const TMap<FName,bool>& GetSkillUnlocks() const;

	float GetSkillCD(const FName& name);

	float GetSkillCD(int index);

	float GetSkillDmg(const FName& name);
};

