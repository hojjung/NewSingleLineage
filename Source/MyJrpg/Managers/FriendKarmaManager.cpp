#include "FriendKarmaManager.h"
#include "MyJrpg/DataTables/FriendTeamTable.h"

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
