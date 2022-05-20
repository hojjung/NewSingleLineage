#include "WidgetItemConvert.h"

#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"

void UWidgetItemConvert::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Inven->Init(UMyLib::GetPlayerInven());

	m_LeftItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_LeftItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);
	
	m_RightItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_RightItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);
	
	m_FuelItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_FuelItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);

	m_CostItem->m_OnDrag.AddUObject(this,&UWidgetItemConvert::OnDrag);
	m_CostItem->m_OnDrop.AddUObject(this,&UWidgetItemConvert::OnDrop);

	m_LeftItem->SetIndex(0);
	m_RightItem->SetIndex(1);
	m_FuelItem->SetIndex(2);
	m_CostItem->SetIndex(3);
	
	m_AryItems.Reset();
	m_AryItems.Add(m_LeftItem);
	m_AryItems.Add(m_RightItem);
	m_AryItems.Add(m_FuelItem);
	m_AryItems.Add(m_CostItem);

	m_LeftItem->SetFocusable(false);
	m_RightItem->SetFocusable(false);
	m_FuelItem->SetFocusable(false);
	m_CostItem->SetFocusable(false);
	
	m_LeftItem->SetDragable(true);
	m_RightItem->SetDragable(true);
	m_FuelItem->SetDragable(true);
	m_CostItem->SetDragable(true);

	m_LeftItem->SetHoldable(false);
	m_RightItem->SetHoldable(false);
	m_FuelItem->SetHoldable(false);
	m_CostItem->SetHoldable(false);
}

void UWidgetItemConvert::SetProgressBar(float v)
{
	m_BarArrow->SetPercent(v);
}

void UWidgetItemConvert::SetFireBar(float v)
{
	m_BarFireTime->SetPercent(v);
}

void UWidgetItemConvert::ClosePanel()
{
	Super::ClosePanel();
	m_ItemConvertInst->m_OnItemConvertInst.Remove(m_Dele);
	m_ItemConvertInst.Reset();
}

void UWidgetItemConvert::UpdatePanel()
{
	m_Inven->OpenPanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_Bag->Init(UMyGameInstance::Get->m_EquipManager->GetBag());
		m_Bag->OpenPanel();
	}

	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_Belt->Init(UMyGameInstance::Get->m_EquipManager->GetBelt());
		m_Belt->OpenPanel();
	}
	
	UpdateElement(m_LeftItem,m_ItemConvertInst->GetLeftItem());
	UpdateElement(m_RightItem,m_ItemConvertInst->GetRightItem());
	UpdateElement(m_FuelItem,m_ItemConvertInst->GetFuelItem());
	UpdateElement(m_CostItem,m_ItemConvertInst->GetCostItem());
}

void UWidgetItemConvert::UpdateElement(UWidgetBaseElement* ele, const FItemSpec& item)
{
	if(item.m_ID.IsNone())
	{
		ele->Clear();
		return ;
	}

	ele->SetItem(item);
}

void UWidgetItemConvert::OnDrag(UWidgetBaseElement* ele)
{
	UItemDDO::GetDDOInst->m_FromInven = m_ItemConvertInst;

	UItemDDO::GetDDOInst->m_nIndex = ele->GetIndex();
}

void UWidgetItemConvert::OnDrop(UWidgetBaseElement* ele)
{
	if(UItemDDO::GetDDOInst->m_FromInven.Get())
	{
		const FItemSpec& ItemSpec = UItemDDO::GetDDOInst->GetItem();
		
		switch (ele->GetIndex())
		{
		case 0:
			if(!m_ItemConvertInst->CheckLeftItemAvailable(ItemSpec))
			{
				return;
			}
			break;
		case 1:
			return;//여기 드랍되면 아예안됨
			break;
		case 2:
			if(!m_ItemConvertInst->CheckFuelItemAvailable(ItemSpec))
			{
				return;
			}
			break;
		case 3:
			if(!m_ItemConvertInst->CheckCostItemAvailable(ItemSpec))
			{
				return;
			}
			break;
		}
		
		m_ItemConvertInst->OnDropItem(ele->GetIndex(),UItemDDO::GetDDOInst->m_FromInven.Get(),UItemDDO::GetDDOInst->m_nIndex);
	}
}

void UWidgetItemConvert::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float Per = m_ItemConvertInst->GetRemainTimePer();

	SetProgressBar(Per);

	float FirePer = m_ItemConvertInst->GetFireRemainTimePer();

	SetFireBar(FirePer);
}

void UWidgetItemConvert::ShowItemConvert(UItemConvertInst* inst)
{
	OpenPanel();
	m_ItemConvertInst = inst;
	m_Dele = m_ItemConvertInst->m_OnItemConvertInst.AddUObject(this, &UWidgetItemConvert::UpdatePanel);
	UpdatePanel();

	const FText& ConvertText = m_ItemConvertInst->GetConvertRow().m_TextConverterName;
	
	m_TextConverter->SetText(ConvertText);

	SetProgressBar(0);
}
