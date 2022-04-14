// All right Reserve 2021 HereticByte


#include "AttachEquipmentBase.h"

// Sets default values
AAttachEquipmentBase::AAttachEquipmentBase()
{
	PrimaryActorTick.bCanEverTick = false;
	m_SocketID = TEXT("Trail_End");
	
}

FName AAttachEquipmentBase::GetSocketName()
{
	return m_SocketID;
}