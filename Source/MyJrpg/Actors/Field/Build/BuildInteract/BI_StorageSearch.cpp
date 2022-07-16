#include "BI_StorageSearch.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UBI_StorageSearch::OnSearchEnd()
{
	m_bIsSearched = true;
	m_Player->CancelInteract();
	UMyGameInstance::Get->m_PlayerStatManager->AddExp(12);
	Super::OnInteract();
}

void UBI_StorageSearch::Init(const TArray<FString>& variable, UInventory* inven)
{
	UBuildInteractBase::Init(variable, inven);

	m_bIsSearched = false;

	if(inven)
	{
		m_Inven	 = inven;
		m_bIsSearched = true;
	}
	else
	{
		m_Inven = NewObject<UInventory>(UMyGameInstance::Get);
		
		int Size = FCString::Atoi(*variable[0]);
		
		m_Inven->Init(Size,NSLOCTEXT("UBI_StorageSearch","StorageName","루트박스"));

		if(variable.Num() % 3 != 0 && variable.Num() >= 4)
		{
			int Iter = 1;
			while (Iter < variable.Num())
			{
				FName ItemID = *variable[Iter];
				
				int Cnt = FCString::Atoi(*variable[Iter + 1]);

				float RandLuck = FCString::Atoi(*variable[Iter + 2]);

				float Rand = FMath::RandRange(0.f, 1.f);

				if(Rand <= RandLuck)
				{
					int Dur = 0;

					const FItemDataRow& ItemData = UMyLib::GetItemData(ItemID);
					
					if(UMyLib::IsEquip(ItemData))
					{
						float MinDur = (float)ItemData.m_nDurability * 0.15f;

						float MaxDur = (float)ItemData.m_nDurability * 0.85f;
						
						Dur = FMath::RandRange(MinDur, MaxDur);
					}
					FItemSpec NewItem(ItemID, Cnt, Dur);


					m_Inven->AddItem(NewItem);
				}
				
				Iter +=3;
			}
		}
		
	}
	m_Player = UMyLib::GetPlayer();
}

bool UBI_StorageSearch::IsEraseable()
{
	return false;
}

void UBI_StorageSearch::OnInteract()
{
	if(m_Player->GetInteracting())
	{
		return;
	}
	if(!m_bIsSearched)
	{
		m_Player->WaitInteract(4.f,FVoidVoid::CreateUObject(this, &UBI_StorageSearch::OnSearchEnd));
		return ;
	}
	Super::OnInteract();
}
