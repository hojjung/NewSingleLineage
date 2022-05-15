// All right Reserve 2021 HereticByte


#include "AttachEquipmentBase.h"

#include "MyJrpg/MyLib.h"

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

void AAttachEquipmentBase::SetActive(bool b)
{
	SetActorHiddenInGame(!b);

	for(auto Comp : GetComponents())
	{
		Comp->SetActive(b);
		Comp->SetComponentTickEnabled(b);
	}
}

const TSubclassOf<UAnimInstance>& AAttachEquipmentBase::GetAnimBP() const
{
	return m_ABP;
}
