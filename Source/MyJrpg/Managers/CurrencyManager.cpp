#include "CurrencyManager.h"
#include "MyJrpg/MyJrpg.h"

void UCurrencyManager::Init(int gold)
{
	m_nGold = gold;
}

int UCurrencyManager::GetGold()
{
	return m_nGold;
}

bool UCurrencyManager::SubGold(int amount)
{
	if(!CheckGoldEnough(amount))
	{
		return false;
	}

	m_nGold -= amount;

	m_GoldChanged.Broadcast();

	return true;
}

void UCurrencyManager::AddGold(int amount)
{
	m_nGold += amount;

	if(m_nGold>INT64_MAX)
	{
		m_nGold = INT64_MAX; 
	}

	m_GoldChanged.Broadcast();
}

bool UCurrencyManager::CheckGoldEnough(int amount)
{
	return m_nGold - amount >= 0;
}
