// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PotionManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UPotionManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnPotionChanged);

	FOnPotionChanged m_OnPotionChanged;

	FOnPotionChanged m_OnUsePotion;
	
public:
	UPotionManager();
	
protected:
	UPROPERTY()
	TArray<UTexture2D*> m_AryIcons;
	
	float m_AryPotionHeal[9];

	int m_AryPotionMaxCount[9];

	int m_nLvHeal;

	int m_nLvCount;

	int m_nCrntCnt;

	float m_fMaxCD;

	float m_fCrntCD;
	
public:
	bool UsePotion();
	
	void SetLevelHeal(int lv);

	void SetLevelMaxCount(int lv);

	float GetValueHeal();

	int GetValueCnt();

	int GetLevelHeal();
	
	int GetLevelMaxCount();

	int GetCrntCnt();

	UTexture2D* GetPotionIcon();

	void Tick(float delta);

	float GetMaxCD();

	void Init();

	void FillCount();
};
