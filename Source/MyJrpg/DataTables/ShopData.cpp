// All right Reserve 2021 HereticByte


#include "ShopData.h"

UDataTable* UShopData::GetShopTable = nullptr;

UDataTable* UShopData::GetBlackSmithTable = nullptr;

UShopData::UShopData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/00_DataTables/Item/VillagePotionShop.VillagePotionShop'"));

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable02(TEXT("DataTable'/Game/00_DataTables/Item/VillageEquipShop.VillageEquipShop'"));
	//
	GetShopTable = FoundTable01.Object;

	GetBlackSmithTable = FoundTable02.Object;
}
