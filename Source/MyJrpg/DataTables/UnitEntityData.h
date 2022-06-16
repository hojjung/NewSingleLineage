#pragma once

#include "ItemData.h"
#include "NPCPaletteDataAsset.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Pawns/Logics/Interact/InteractBase.h"
#include "UObject/NoExportTypes.h"
#include "UnitEntityData.generated.h"


class AMonsterPawn;
class UAI_LogicBase;
class USensor_LogicBase;
UCLASS()
class MYJRPG_API UUnitEntityData : public UObject
{
	GENERATED_BODY()
public:
	UUnitEntityData();
	
	static UDataTable* GetNpcUnitTable;
};

USTRUCT(BlueprintType)
struct FAttach
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* m_BackMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* m_LeftMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* m_RightMesh;
};


UCLASS(Blueprintable, hidecategories = (Object, Actor, Advanced, Navigation))
class MYJRPG_API UUnitEntityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public://Visual
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_BodyMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_RotYawOffset;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttach m_Attach; 

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Unit"), GetFName());
	}

	virtual USkeletalMesh* GetSkMesh() const;

	virtual UAnimMontage* GetBaseAtkAnim() const;
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
struct FDropRewardItem
{
	GENERATED_BODY()

public:
	FDropRewardItem(FName id, int cnt)
	{
		m_Item.RowName = id;

		m_nExpectDropCount = cnt;
	}
	FDropRewardItem()
	{
		
	}

public:
	UPROPERTY(EditDefaultsOnly)
	FItemDataHandle m_Item;
	UPROPERTY(EditDefaultsOnly)
	int m_nMaxStack = 1;
	UPROPERTY(EditDefaultsOnly)
	int m_nExpectDropCount=5;//5번에 한번,20% 드랍이란뜻임
};

USTRUCT(BlueprintType)
struct FNpcUnitEntityRow : public FUnitEntityRow
{
	GENERATED_BODY()

public:
	FNpcUnitEntityRow();
	
public://스텟과 보상
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMonsterPawn> m_ClassActor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "100"))
	float m_fAtkRange = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bIsBoss = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bIsRespawn = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_FriendTeamID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_TalkID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAI_LogicBase> m_ClassAI_Logic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USensor_LogicBase> m_ClassAI_Sensor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInteractBase> m_ClassInteract;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ClassInteract != nullptr", EditConditionHides))
	FString m_InteractVariable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* m_Bullet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fBulletScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "34" ,UIMax = "88"))
	float m_fCapsuleRadius = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fShadowScale = 6.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "1"))
	float m_fExp = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDropRewardItem> m_AryDropItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* m_TakeHitEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* m_TakeHitSound;
};