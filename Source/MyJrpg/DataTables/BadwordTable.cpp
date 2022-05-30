#include "BadwordTable.h"

UDataTable* UBadwordTable::GetBadwordTable = nullptr;

UBadwordTable::UBadwordTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable(
		TEXT("DataTable'/Game/00_DataTables/BadwordTable.BadwordTable'"));
	//
	GetBadwordTable = FoundTable.Object;
}

void UBadwordTable::Init()
{
	GetBadwordTable->GetAllRows<FBadWordDataRow>("", m_AryBadwordList);
}

void UBadwordTable::ChatFilterBadWord(FString& outChatWant)
{
	for (FBadWordDataRow* BadWord : m_AryBadwordList)
	{
		outChatWant = outChatWant.Replace(*BadWord->m_Badword,TEXT("*"));
	}

	FString PercentPrevent = TEXT("%");
	outChatWant = outChatWant.Replace(*PercentPrevent,TEXT("%%")); //퍼센트
}

bool UBadwordTable::NicknameHasBadWord(const FString& nameWant)
{
	const TArray<TCHAR>& AryChar = nameWant.GetCharArray();

	for (TCHAR wChar : AryChar)
	{
		if (L' ' <= wChar && wChar <= L'/') //특수문자 잡기
		{
			return true;
		}
		if (L':' <= wChar && wChar <= L'@') //특수문자 잡기
		{
			return true;
		}
		if (L'[' <= wChar && wChar <= L'`') //특수문자 잡기
		{
			return true;
		}
		if (L'{' <= wChar && wChar <= L'~') //특수문자 잡기
		{
			return true;
		}
		if (L'ㄱ' <= wChar && wChar <= L'ㅣ')//미완성한글 잡기
		{
			return true;
		}
	}

	for (auto* BadWord : m_AryBadwordList)
	{
		if (nameWant.Contains(*BadWord->m_Badword))
		{
			return true;
		}
	}
	return false;
}
