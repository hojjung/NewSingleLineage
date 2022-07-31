#include "WidgetAssemble.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"

void UWidgetAssemble::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_ArySlots.Reset(10);
	m_ArySlots.Add(m_Slot01);
	m_ArySlots.Add(m_Slot02);
	m_ArySlots.Add(m_Slot03);
	m_ArySlots.Add(m_Slot04);
	m_ArySlots.Add(m_Slot05);
	m_ArySlots.Add(m_Slot06);
	m_ArySlots.Add(m_Slot07);
	m_ArySlots.Add(m_Slot08);
	m_ArySlots.Add(m_Slot09);
	m_ArySlots.Add(m_Slot10);
	
	int Index = 0;
	for(UWidgetCraftCostElement* Ele : m_ArySlots)
	{
		Ele->m_nIndex = Index;
		Ele->SetVisibility(ESlateVisibility::Collapsed);
		Ele->m_GetStackFuncPtr.BindUObject(this, &UWidgetAssemble::GetStack);
		Ele->GetEle()->SetDragable(false);
		Ele->GetEle()->m_OnDrop.AddUObject(this, &UWidgetAssemble::OnDrop);
		
		Index++;
	}
	m_BtnPutAll->OnClicked.AddDynamic(this, &UWidgetAssemble::OnPutAll);
	m_BtnComplete->OnClicked.AddDynamic(this, &UWidgetAssemble::OnComplete);
}


void UWidgetAssemble::UpdateSlots()
{
	int Index = 0;
	for(const FItemSpec& ItemIn : m_AssembleInst.Get()->GetAryItems())
	{
		m_ArySlots[Index]->SetCraftCost(m_AssembleInst.Get()->GetCraftData().m_AryCostItem[Index]);
		Index++;
	}
}

int UWidgetAssemble::GetStack(int index)
{
	const FName& ID = m_AssembleInst->GetCraftData().m_AryCostItem[index].m_ItemDataRowHandle.RowName;

	const FItemSpec& ItemSpec = m_AssembleInst->GetItemConstRef(index);

	if(UMyLib::IsEquip(ID))
	{
		if(ItemSpec.m_ID.IsNone())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return ItemSpec.m_nLvStack;
}

void UWidgetAssemble::ShowAssemble(UAssembleInst* assemble_inst)
{
	m_AssembleInst = assemble_inst;

	m_TextName->SetText(m_AssembleInst->GetInvenName());

	m_ImgIcon->SetBrushFromSoftTexture(assemble_inst->GetCraftData().m_Icon);

	int Index = 0;
	for(const FCraftItemCost& Cost : m_AssembleInst.Get()->GetCraftData().m_AryCostItem)
	{
		m_ArySlots[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Index++;
	}
	m_DeleHandle = m_AssembleInst->m_OnInvenChanged.AddUObject(this, &UWidgetAssemble::UpdateSlots);
	UpdateSlots();
}

int UWidgetAssemble::GetRemainNeedItemCount(int index)
{
	int RemainItemNeed = 0;

	const FCraftItemCost& ItemCost = m_AssembleInst.Get()->GetCraftData().m_AryCostItem[index];

	bool IsEquip = UMyLib::IsEquip(ItemCost.m_ItemDataRowHandle.RowName);

	if(IsEquip && m_AssembleInst->GetItemConstRef(index).m_ID.IsNone())
	{
		RemainItemNeed = 1;
	}
	else
	{
		RemainItemNeed = ItemCost.m_nStackOrLevel - m_AssembleInst->GetItemConstRef(index).m_nLvStack;	
	}

	return RemainItemNeed;
}

void UWidgetAssemble::TryPutItem(int index, UInventory* inven, const FCraftItemCost& cost, int outRemain)
{
	int ItemHave = inven->GetItemCount(cost.m_ItemDataRowHandle.RowName, cost.m_nStackOrLevel);

	if(ItemHave > 0)
	{
		int RemoveCount = FMath::Min(ItemHave, outRemain);

		if(UMyLib::IsEquip(cost.m_ItemDataRowHandle.RowName))
		{
			inven->RemoveItem(cost.m_ItemDataRowHandle.RowName, cost.m_nStackOrLevel);

			m_AssembleInst->PutItem(index, cost, 0);
		}
		else
		{
			inven->RemoveItem(cost.m_ItemDataRowHandle.RowName, RemoveCount);

			m_AssembleInst->PutItem(index, cost, RemoveCount);
		}
		outRemain -= RemoveCount;
	}
}

void UWidgetAssemble::OnPutAll()
{
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;
	
	int Index = 0;

	int RemainItemNeed = 0;

	for(const FCraftItemCost& Cost : m_AssembleInst.Get()->GetCraftData().m_AryCostItem)
	{
		RemainItemNeed = GetRemainNeedItemCount(Index);

		if(RemainItemNeed <= 0)
		{
			Index++;
			continue;
		}
		
		TryPutItem(Index, UMyLib::GetPlayerInven(), Cost, RemainItemNeed);

		if(RemainItemNeed <= 0)
		{
			Index++;
			continue;
		}
		
		if(Equip->GetBag())
		{
			TryPutItem(Index, Equip->GetBag(), Cost, RemainItemNeed);
			
			if(RemainItemNeed <= 0)
			{
				Index++;
				continue;
			}
		}
		Index++;
	}
	m_AssembleInst->UpdateInventory();
}

void UWidgetAssemble::OnComplete()
{
	if(m_AssembleInst->TryComplete())
	{
		m_OnComplete.Execute();
		
	}
}

void UWidgetAssemble::ClosePanel()
{
	m_AssembleInst->m_OnInvenChanged.Remove(m_DeleHandle);
}

void UWidgetAssemble::OnDrag(UWidgetBaseElement* ele)
{
	UItemDDO::GetDDOInst->m_FromAssemble = m_AssembleInst;

	UItemDDO::GetDDOInst->m_FromInven = nullptr;

	UItemDDO::GetDDOInst->m_nIndex = ele->GetIndex();
}

void UWidgetAssemble::OnDrop(UWidgetBaseElement* ele)
{
	if(UItemDDO::GetDDOInst->m_FromInven.Get())
	{
		const FItemSpec& ItemSpec = UItemDDO::GetDDOInst->GetItem();

		int Index = ele->GetIndex();

		if(!m_AssembleInst->IsSlotPutable(Index, ItemSpec))
		{
			return;
		}
		m_AssembleInst->OnDropItem(ele->GetIndex(),UItemDDO::GetDDOInst->m_FromInven.Get(),UItemDDO::GetDDOInst->m_nIndex);
	}
}
