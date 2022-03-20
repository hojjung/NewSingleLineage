#pragma once

#include "ItemData.h"
#include "NPCPaletteDataAsset.h"
#include "MyJrpg/MyJrpg.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DropData.h"
#include "UnitEntityData.generated.h"


class UAI_LogicBase;
class USensor_LogicBase;
UCLASS()
class MYJRPG_API UUnitEntityData : public UObject
{
	GENERATED_BODY()
public:
	UUnitEntityData();
	
	static UDataTable* GetPlayerUnitTable;
	
	static UDataTable* GetNpcUnitTable;
};


UCLASS(Blueprintable, hidecategories = (Object, Actor, Advanced, Navigation))
class MYJRPG_API UUnitEntityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public://Visual
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText m_UnitName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* m_BodyMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_SpawnAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_TookHitMontage;
};

USTRUCT(BlueprintType)
struct FUnitEntityRow : public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UUnitEntityAsset> m_UnitDataAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FStatGroup m_StatTable;
};

USTRUCT(BlueprintType)
struct FNpcUnitEntityRow : public FUnitEntityRow
{
	GENERATED_BODY()
public://스텟과 보상
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0"))
	float m_fAtkRange = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAI_LogicBase> m_ClassAI_Logic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USensor_LogicBase> m_ClassAI_Sensor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* m_Bullet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fBulletScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "1"))
	float m_fExp = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0"))
	float m_fGold = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "1"))
	float m_fMinSpawnTime = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "2"))
	float m_fMaxSpawnTime = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDropRewardItem> m_AryDropItem;
};

USTRUCT(BlueprintType)
struct FPlayerSkillAnim
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FName m_SkillID = TEXT("Same As Skill Table");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> m_SkillMotion = nullptr;
};

USTRUCT(BlueprintType)
struct FPlayerUnitEntityRow : public FUnitEntityRow//모든 동료
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fMeleeRange = 200;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fRangeRange = 900;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> m_StanceSword;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> m_StanceMage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerSkillAnim> m_ArySkillAnims;
};
//
USTRUCT(BlueprintType)
struct FUnitSpec//배틀액터 공통으로 사용하는 객체,그와 동시에 저장도 됨
{
	GENERATED_BODY()

public:
	FUnitSpec(): m_nCurrentExp(0), m_Stats()
	{
		m_nLevel = 1;
	}

	FUnitSpec(const FName& nameID,int level,int remainExp,const FPlayerUnitEntityRow* dataRow)
	{
		m_UnitID = nameID.ToString();
		m_nLevel = level;
		m_nCurrentExp=remainExp;
	}

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString m_UnitID;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nLevel;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCurrentExp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FStatGroup m_Stats;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FString> m_AryEquipItems;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FString> m_AryEquipSkills;//스킬레벨 보단 상위호환 스킬로 갈아 끼우자
	//스킬을 저장할순 있다만,스킬의 인스턴스는 어디에?
public:
	bool IsUnlocked() const
	{
		return m_nLevel > 0;
	}
};