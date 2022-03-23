#pragma once

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Actors/Equipments/AttachEquipmentBase.h"
#include "MyJrpg/Items/ItemExecuteBase.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"



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
		m_MoveSpeed = 600;
		m_CriPer = 0;
		m_DmgReduce = 0;
		m_CriDmg = 1.5f;
	}

	FStatGroup(float initAll)//for multiplier
	{
		m_nAvoid= initAll;
		m_nAccu = initAll;
		m_MaxHp = initAll;
		m_Hp = initAll;
		m_Dmg = initAll;
		m_AtkSpeed = initAll;
		m_MoveSpeed = initAll;
		m_CriPer = initAll;
		m_CriDmg = initAll;
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
	
	FStatGroup operator+(const FStatGroup& stat_group);
	
	FStatGroup operator*(const FStatGroup& stat_group);

	FStatGroup& operator+=(const FStatGroup& stat_group);

	FStatGroup& operator-=(const FStatGroup& stat_group);
	
};
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
struct FColorDataRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FSlateColor m_Color;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_GlowTexture;
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
	int m_nStackCount=1;

	TSoftObjectPtr<UTexture2D> GetItemIcon() const;
};

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText m_TextShowingName;
	UPROPERTY(EditDefaultsOnly,meta = (MultiLine="true"))
	FText m_TextDesc;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> m_ItemIcon;
	UPROPERTY(EditDefaultsOnly)
	EEquipSlotType m_ItemType;
	UPROPERTY(EditDefaultsOnly)
	int m_nSellValue = 50;
	UPROPERTY(EditDefaultsOnly)
    int m_nBuyValue = 50;
	UPROPERTY(EditDefaultsOnly)
	FColorDataHandle m_ColorHandle;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	FStatGroup m_EquipStats;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	TSubclassOf<AAttachEquipmentBase> m_ClassEquip;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	TSoftObjectPtr<UParticleSystem> m_EffectRange;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::Weapon", EditConditionHides))
	TSubclassOf<UItemExecuteBase> m_ClassExeItem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FCraftItemCost> m_AryCostItem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCraftCost=1;
};