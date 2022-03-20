// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionManager.h"

#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

UPotionManager::UPotionManager()
{
	m_AryIcons.Init(nullptr,9);
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found00(TEXT("Texture2D'/Game/Sprites/UI/Potion/a.a'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found01(TEXT("Texture2D'/Game/Sprites/UI/Potion/b.b'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found02(TEXT("Texture2D'/Game/Sprites/UI/Potion/c.c'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found03(TEXT("Texture2D'/Game/Sprites/UI/Potion/d.d'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found05(TEXT("Texture2D'/Game/Sprites/UI/Potion/f.f'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found06(TEXT("Texture2D'/Game/Sprites/UI/Potion/g.g'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found07(TEXT("Texture2D'/Game/Sprites/UI/Potion/h.h'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D>Found08(TEXT("Texture2D'/Game/Sprites/UI/Potion/i.i'"));
	
	m_AryIcons[0] = Found00.Object;
	m_AryIcons[1] = Found00.Object;
	m_AryIcons[2] = Found00.Object;
	m_AryIcons[3] = Found00.Object;
	m_AryIcons[4] = Found00.Object;
	m_AryIcons[5] = Found00.Object;
	m_AryIcons[6] = Found00.Object;
	m_AryIcons[7] = Found00.Object;
	m_AryIcons[8] = Found00.Object;
	
	m_AryPotionHeal[0] = 0.25f;
	m_AryPotionHeal[1] = 0.30f;
	m_AryPotionHeal[2] = 0.35f;
	m_AryPotionHeal[3] = 0.40f;
	m_AryPotionHeal[4] = 0.45f;
	m_AryPotionHeal[5] = 0.50f;
	m_AryPotionHeal[6] = 0.55f;
	m_AryPotionHeal[7] = 0.60f;
	m_AryPotionHeal[8] = 0.65f;

	m_AryPotionMaxCount[0] = 3;
	m_AryPotionMaxCount[1] = 4;
	m_AryPotionMaxCount[2] = 5;
	m_AryPotionMaxCount[3] = 6;
	m_AryPotionMaxCount[4] = 7;
	m_AryPotionMaxCount[5] = 8;
	m_AryPotionMaxCount[6] = 9;
	m_AryPotionMaxCount[7] = 10;
	m_AryPotionMaxCount[8] = 11;

	m_nLvHeal = 0;
	m_nLvCount = 0;
	m_nCrntCnt = 0;
	m_fMaxCD = 5;
}

void UPotionManager::Init()
{
	SetLevelHeal(0);
	SetLevelMaxCount(0);
	FillCount();
}

void UPotionManager::FillCount()
{
	m_nCrntCnt = GetValueCnt();
}

void UPotionManager::SetLevelHeal(int lv)
{
	m_nLvHeal = lv;
}

void UPotionManager::SetLevelMaxCount(int lv)
{
	m_nLvCount = lv;
}

float UPotionManager::GetValueHeal()
{
	return m_AryPotionHeal[GetLevelHeal()];
}

int UPotionManager::GetValueCnt()
{
	return m_AryPotionMaxCount[GetLevelMaxCount()];
}

int UPotionManager::GetLevelHeal()
{
	return m_nLvHeal;
}

int UPotionManager::GetLevelMaxCount()
{
	return m_nLvCount;
}

int UPotionManager::GetCrntCnt()
{
	return m_nCrntCnt;
}

UTexture2D* UPotionManager::GetPotionIcon()
{
	return m_AryIcons[GetLevelHeal()];
}

void UPotionManager::Tick(float delta)
{
	m_fCrntCD -= delta;
}

bool UPotionManager::UsePotion()
{
	if (m_fCrntCD > 0.f)
	{
		return false;
	}
	if (GetCrntCnt() < 1)
	{
		return false;
	}

	m_nCrntCnt--;

	float HealV = UMyGameInstance::Get->m_PlayerStatManager->GetStat().m_MaxHp * GetValueHeal();

	UMyLib::GetPlayer()->TakeHeal(HealV);
	
	m_fCrntCD = GetMaxCD();
	
	m_OnUsePotion.Broadcast();
	
	m_OnPotionChanged.Broadcast();

	return true;
}

float UPotionManager::GetMaxCD()
{
	return m_fMaxCD;
}


