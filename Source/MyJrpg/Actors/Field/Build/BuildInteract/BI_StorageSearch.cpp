#include "BI_StorageSearch.h"
#include "MyJrpg/MyLib.h"

UBI_StorageSearch::UBI_StorageSearch()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FoundAnim(TEXT("AnimMontage'/Game/09_SharedAnimations/Loot_Anim_Set/Loot_Corpse_GrabItem_Montage.Loot_Corpse_GrabItem_Montage'"));
	m_Anim = FoundAnim.Object;
}

void UBI_StorageSearch::OnSearchEnd()
{
	m_bIsSearched = true;
	m_Player->SetInteracting(false);
}

void UBI_StorageSearch::Init()
{
	Super::Init();

	m_bIsSearched = false;
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
		m_Player->WaitInteract(m_Anim, 4.f,FVoidVoid::CreateUObject(this, &UBI_StorageSearch::OnSearchEnd));
		return ;
	}
	Super::OnInteract();
}
