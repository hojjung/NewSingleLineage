#include "FriendTeamTable.h"

UDataTable* UFriendTeamTable::GetFriendTeamTable = nullptr;

UFriendTeamTable::UFriendTeamTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable(TEXT("DataTable'/Game/00_DataTables/Karma/FriendKarmaTable.FriendKarmaTable'"));
	//
	GetFriendTeamTable = FoundTable.Object;
}
