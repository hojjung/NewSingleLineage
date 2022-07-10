#include "BI_Assemble.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"


void UBI_Assemble::Init(const TArray<FString>& variable, UInventory* inven)
{
	Super::Init(variable, inven);

	if(inven)
	{
		m_ItemHolder = Cast<UAssembleInst>(inven);	
	}
	else
	{
		m_ItemHolder = NewObject<UAssembleInst>(UMyGameInstance::Get);

		const FBuildDataRow& BuildData = Cast<AStructureActor>(GetOuter())->GetBuildData();

		const FBuildDataRow& NextBuildData = *UBuildData::GetBuildTable->FindRow<FBuildDataRow>(BuildData.m_NextUpgradeActorID, "");
		
		m_ItemHolder->SetCraftItem(this, NextBuildData);
	}
}

bool UBI_Assemble::IsEraseable()
{
	return m_ItemHolder->IsEmpty();
}

void UBI_Assemble::OnInteract()
{
	UMyLib::GetCanvas()->OpenAssemble(m_ItemHolder);
}

UInventory* UBI_Assemble::GetItemHolder()
{
	return m_ItemHolder;
}
