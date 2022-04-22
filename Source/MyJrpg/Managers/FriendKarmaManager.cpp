#include "FriendKarmaManager.h"
#include "MyJrpg/DataTables/FriendTeamTable.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void UFriendKarmaManager::Init()
{
	TArray<FFriendDataRow*> AryData;
	
	UFriendTeamTable::GetFriendTeamTable->GetAllRows("",AryData);

	TArray<FName> AryKey = UFriendTeamTable::GetFriendTeamTable->GetRowNames();

	m_MapKarma.Reset();
	
	int Iter = 0;

	for(const FName& KeyName : AryKey)
	{
		m_MapKarma.Add(KeyName, AryData[Iter]->m_nInitKarma);

		Iter++;
	}
}

int UFriendKarmaManager::GetKarma(const FName&& id)
{
	return GetKarma(id);
}

int UFriendKarmaManager::GetKarma(const FName& id)
{
	if(!m_MapKarma.Contains(id))
	{
		return 0;
	}
	return m_MapKarma[id];
}

bool UFriendKarmaManager::IsFoe(const ACombatUnitPawn* Other)
{
	return GetUnitKarma(Other) == EKarma::Hate;
}

EKarma UFriendKarmaManager::GetUnitKarma(const ACombatUnitPawn* Other)
{
	const FName& TeamID = Other->GetTeamID();

	int Karma = GetKarma(TeamID);

	if(Karma >= FGlobalVariable::KARMA_FRIEND)
	{
		return EKarma::Friendly;
	}
	else if(Karma <= FGlobalVariable::KARMA_FOE)
	{
		return EKarma::Hate;
	}
	return  EKarma::Neutral;
}

void UFriendKarmaManager::IncreaseKarma(const FName& id, int v)
{
	int& KarmaValue = m_MapKarma[id];

	KarmaValue += v;

	KarmaValue = FMath::Min(KarmaValue, FGlobalVariable::KARMA_MAX);
}

void UFriendKarmaManager::DecreaseKarma(const FName& id, int v)
{
	int& KarmaValue = m_MapKarma[id];

	KarmaValue -= v;

	KarmaValue = FMath::Max(KarmaValue, FGlobalVariable::KARMA_MIN);
}
