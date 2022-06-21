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

		if(variable.Num() % 2 != 0 && variable.Num() >= 3)
		{
			int Iter = 1;
			while (Iter < variable.Num())
			{
				FName ItemID = *variable[Iter];
				
				int Cnt = FCString::Atoi(*variable[Iter + 1]);

				int RandLuck = FMath::RandRange(0, Cnt);

				if(RandLuck > 0)
				{
					FItemSpec NewItem(ItemID, RandLuck);

					m_Inven->AddItem(NewItem);
				}
				
				Iter+=2;
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
