// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitEntityData.h"
#include "HumanAsset.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UHumanAsset : public UUnitEntityAsset
{
	GENERATED_BODY()

public://Visual
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_HeadMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_HatMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_TorsoArmorMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_GlovesMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_LegsMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EStanceType m_StanceType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> m_AryAdditionalAnims;
public:
	virtual USkeletalMesh* GetSkMesh() const override;

	UAnimMontage* GetStanceAnim(EStanceType t) const;

	virtual UAnimMontage* GetBaseAtkAnim() const override;
};
