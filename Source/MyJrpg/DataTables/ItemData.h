#pragma once

#include "NPCPaletteDataAsset.h"
#include "MyJrpg/MyJrpg.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"


class UEE_Base;
class UItemExecuteBase;
class UOptionBase;
class AItemActor;
UCLASS()
class MYJRPG_API UItemData : public UObject
{
	GENERATED_BODY()
public:
	UItemData();

	static UDataTable* GetItemTable;

	static UDataTable* GetColorTable;
};


USTRUCT(BlueprintType)
struct FStatGroup 
{
	GENERATED_BODY()

public:
	FStatGroup()
	{
		m_nAccu = 10;
		m_nAvoid = 0;
		m_MaxHp = 100;
		m_Hp = 100;
		m_Dmg = 10;
		m_AtkSpeed = 1.0f;
		m_MoveSpeed = 300;
		m_CriPer = 0;
		m_DmgReduce = 0;
		m_CriDmg = 1.5f;
	}

	FStatGroup(float initAll)//for multiplier
	{
		m_nAvoid    = initAll;
		m_nAccu     = initAll;
		m_MaxHp     = initAll;
		m_Hp        = initAll;
		m_Dmg       = initAll;
		m_AtkSpeed  = initAll;
		m_MoveSpeed = initAll;
		m_CriPer    = initAll;
		m_CriDmg    = initAll;
		m_DmgReduce = initAll;
	}

public:
	UPROPERTY(EditDefaultsOnly)
	int m_nAccu;
	UPROPERTY(EditDefaultsOnly)
	int m_nAvoid;
	UPROPERTY(EditDefaultsOnly)
	float m_MaxHp;
	UPROPERTY(BlueprintReadOnly)
	float m_Hp;
	UPROPERTY(EditDefaultsOnly)
	float m_Dmg;
	UPROPERTY(EditDefaultsOnly)
	int m_DmgReduce;
	UPROPERTY(EditDefaultsOnly)
	float m_CriPer;
	UPROPERTY(EditDefaultsOnly)
	float m_CriDmg;
	UPROPERTY(EditDefaultsOnly)
	float m_AtkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float m_MoveSpeed;
	
	FStatGroup  operator + (const FStatGroup& stat_group) const;
	
	FStatGroup  operator * (const FStatGroup& stat_group) const;

	FStatGroup  operator * (float m) const;

	FStatGroup  operator * (int m) const;

	FStatGroup& operator+= (const FStatGroup& stat_group);

	FStatGroup& operator-=(const FStatGroup& stat_group);
};

USTRUCT(BlueprintType)
struct FColorDataRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FSlateColor m_Color;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_GlowTexture;
	UPROPERTY(EditAnywhere)
	UParticleSystem* m_HeadAura;
	UPROPERTY(EditAnywhere)
	UParticleSystem* m_Bullet;
	UPROPERTY(EditAnywhere)
	UParticleSystem* m_RangeAura;
	UPROPERTY(EditAnywhere)
	UParticleSystem* m_SwordTrail;
	UPROPERTY(EditAnywhere)
	UParticleSystem* m_SwordAura;
};

USTRUCT(BlueprintType)
struct FColorDataHandle : public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()

public:
	FColorDataHandle()
	{
		DataTable = UItemData::GetColorTable;
		RowName = TEXT("Default");
	}
};

USTRUCT(BlueprintType)
struct FItemDataHandle : public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()

public:
	FItemDataHandle()
	{
		DataTable = UItemData::GetItemTable;
	}
};

USTRUCT(BlueprintType)//플레이어는 처음부터 무엇을 제작할수 있는지는 전부 확인 가능하다
struct FCraftItemCost
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDataHandle m_ItemDataRowHandle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nStackOrLevel=1;

	TSoftObjectPtr<UTexture2D> GetItemIcon() const;
};

USTRUCT(BlueprintType)
struct FDropData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_ZoneUniqueID;
	UPROPERTY(EditDefaultsOnly)
	int m_nExpectDropCount=5;//5번에 한번,20% 드랍이란뜻임
};

USTRUCT(BlueprintType)
struct FCraftable :  public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCraftLevelLimit=1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FCraftItemCost> m_AryCostItem;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EStanceType m_Stance;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> m_MeshRight;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> m_MeshLeft;
};

USTRUCT(BlueprintType)
struct FItemDataRow : public FCraftable//FCraftable 상속?
{
	GENERATED_USTRUCT_BODY()

public:
	FItemDataRow();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AItemActor> m_ClassActor;
	UPROPERTY(EditDefaultsOnly)
	EEquipSlotType m_ItemType;
	UPROPERTY(EditDefaultsOnly)
	int m_nPlayerEarnGoldSell = 50;
	UPROPERTY(EditDefaultsOnly)
    int m_nPlayerSpentGoldBuy = 50;
	UPROPERTY(EditDefaultsOnly)
	FColorDataHandle m_ColorHandle;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None && m_ItemType != EEquipSlotType::Weapon", EditConditionHides))
	TSoftObjectPtr<USkeletalMesh> m_ArmorMesh;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	bool m_bIsRange = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	FStatGroup m_EquipStats;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	FStatGroup m_EnchantStats;//this * level = enchant
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType == EEquipSlotType::Weapon", EditConditionHides))
	FWeaponData m_WeaponData;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	TArray<TSubclassOf<UOptionBase>> m_Options;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	TArray<TSubclassOf<UEE_Base>> m_EquipEffects;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::Weapon", EditConditionHides))
	TSubclassOf<UItemExecuteBase> m_ClassExeItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int m_nMaxStack = 2;
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> m_AryTraderIDs;
	UPROPERTY(EditDefaultsOnly)
	TArray<FDropData> m_AryDropDatas;
};