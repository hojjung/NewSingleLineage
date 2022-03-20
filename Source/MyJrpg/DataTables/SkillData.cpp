#include "SkillData.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* USkillData::GetSkillTable = nullptr;

USkillData::USkillData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/SkillData.SkillData'"));
	//
	GetSkillTable = FoundTable01.Object;
}

FSkillDataRowHandle::FSkillDataRowHandle()
{
	DataTable = USkillData::GetSkillTable;
}

