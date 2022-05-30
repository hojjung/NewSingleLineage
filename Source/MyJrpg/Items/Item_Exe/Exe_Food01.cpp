#include "Exe_Food01.h"
#include "MyJrpg/DataTables/BuffTable.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_Food01::Use(FItemSpec& item, UInventory* inven) const
{
	const FBuffDataRow* DataFound = UBuffTable::GetBuffTable->FindRow<FBuffDataRow>(item.m_ID,TEXT("UExe_Food01::Use Fail Key Find"));
	
	UMyGameInstance::Get->m_PlayerStatManager->AddBuff(DataFound);
}
