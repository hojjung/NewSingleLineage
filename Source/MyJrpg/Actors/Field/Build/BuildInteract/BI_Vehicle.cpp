#include "BI_Vehicle.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"


void UBI_Vehicle::Init(const TArray<FString>& variable, UInventory* inven)
{
	Super::Init(variable, inven);
	
	// if(inven)
	// {
	// 	m_Inven	 = inven;
	// }
	// else
	// {
	// 	m_Inven = NewObject<UInventory>(UMyGameInstance::Get);
	// 	
	// 	int Size = FCString::Atoi(*variable[0]);
	// 	
	// 	m_Inven->Init(Size,NSLOCTEXT("UBI_Storage","StorageName","창고"));
	// }
	// m_Player = UMyLib::GetPlayer();
}

bool UBI_Vehicle::IsEraseable()
{
	return true;//m_Inven->GetUsingSlotCount() <= 0;
}

void UBI_Vehicle::OnInteract()
{
	//UMyLib::GetCanvas()->OpenVehicle(m_Inven);
}

bool UBI_Vehicle::IsVehicleAble()
{
	return true;//Need Fuel
}
