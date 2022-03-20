// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Templates/SubclassOf.h"

#include "SkillData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkillData : public UObject
{
	GENERATED_BODY()
public:
	USkillData();
	
	static UDataTable* GetSkillTable;
};

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bCanMove = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bIsPassive = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_SkillIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_SkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_SkillDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColorDataHandle m_TierColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USkillBase> m_ClassSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fSkillCooldown = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fDuration = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fDamage = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fRange = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fRadius = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector m_BoxHalfSize = FVector(50);
};

//핸들 만들어서 엔티티 테이블이 가질수있게 해줘야함
USTRUCT(BlueprintType)
struct FSkillDataRowHandle : public FDataTableRowHandle
{
	GENERATED_BODY()

public:
	FSkillDataRowHandle();
};