#include "CurrencyData.h"

UDataTable* UCurrencyData::GetCurrencyTable = nullptr;
//

UCurrencyData::UCurrencyData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Item/CurrencyTable.CurrencyTable'"));

	GetCurrencyTable = FoundTable01.Object;
}

FCurrencyDataHandle::FCurrencyDataHandle()
{
	DataTable = UCurrencyData::GetCurrencyTable;
}


