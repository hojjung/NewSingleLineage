// All right Reserve 2021 HereticByte


#include "WidgetItemInfo.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetItemInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nEraseAmount = 1;

	m_ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_ItemIcon->SetMyInteractable(false);

	m_ItemIcon->Clear();
	
	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnClose);

	m_BtnEraseItem->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnErase);

	m_BtnEnchant->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnEnchant);
}

void UWidgetItemInfo::SetCollecItemInfo(const FName& collecID, int index, const FName& oID)
{
	m_CollecID = collecID;

	m_nCollecIndex = index;

	SetItemInfo(EItemInfo::Collection,oID,nullptr);
}

void UWidgetItemInfo::SetItemInfo(EItemInfo info,const FName& oID,UInventory* inven)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	for(UWidgetCollecStatChild* Op : m_AryOptions)
	{
		Op->RemoveFromParent();
	}

	m_Inven = inven;
	
	m_AryOptions.Reset();
	
	m_ItemKey = oID;

	m_ItemIcon->Init(EPanelType::Inven,m_Inven.Get());

	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemKey);

	EItemType Type = UMyLib::GetItemType(ItemData);

	m_ItemIcon->UpdateElement(m_ItemKey);
	
	m_TextItemName->SetText(ItemData.m_TextShowingName);

	m_TextItemDesc->SetText(ItemData.m_TextDesc);

	if (Type == EItemType::Equip)
	{
		int Lv = m_Inven.Get() ? m_Inven.Get()->GetItemLevel(oID) : 0;
		
		m_BtnEraseItem->SetIsEnabled(!UMyLib::GetEquip()->IsItemEquipped(m_ItemKey));
		
		UpdateStat(m_ItemKey,Lv);
	}
	
	SetTypeInfo(info,Type,ItemData);
}


void UWidgetItemInfo::SetTypeInfo(EItemInfo info, EItemType type, const FItemDataRow& ItemData)
{
	m_BtnEnchant->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnEraseItem->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnRegister->SetVisibility(ESlateVisibility::Collapsed);
	
	switch (info)
	{
	case EItemInfo::Inven:
		m_BtnEraseItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UpdateEnchantBtn();
		break;
	case EItemInfo::Market:
		break;
	case EItemInfo::Collection:
		UpdateRegisterBtn();
		UpdateEnchantBtn();
		break;
	case EItemInfo::QuestReward:
		break;
	case EItemInfo::Craft:
		break;
	case EItemInfo::Shop:
		break;
	default: ;
	}	
}	

void UWidgetItemInfo::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetItemInfo::OnErase()
{
	if(UMyLib::GetItemType(m_ItemKey) == EItemType::Equip)
	{
		EraseConfirm();
	}
	else
	{
		UWidgetStackCalculator* Calc = UMyLib::GetCanvas()->OpenCalculator(0);
		Calc->m_OnGetMax.BindUObject(this, &UWidgetItemInfo::GetMax);
		Calc->m_OnNumberAccept.AddUObject(this,&UWidgetItemInfo::EraseConfirm);
	}
	OnClose();
}


int UWidgetItemInfo::GetMax()
{
	return m_Inven.Get()->GetItemStack(m_ItemKey);
}

void UWidgetItemInfo::EraseConfirm(int am)
{
	m_Inven.Get()->RemoveItem(m_ItemKey,am);
}

void UWidgetItemInfo::EraseConfirm()
{
	m_Inven.Get()->RemoveEquipItem(m_ItemKey);
}

void UWidgetItemInfo::UpdateEnchantBtn()//가지고있으면 해당 인벤으로
 {
 	m_BtnEnchant->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
 
 	bool HasItem = false;
 
 	if (UMyLib::IsEquip(m_ItemKey))
 	{
 		HasItem = UMyLib::FindEquipItem(m_ItemKey,0) != nullptr;
 	}
 	else
 	{
 		HasItem = UMyLib::FindMiscItem(m_ItemKey) != nullptr;
 	}
 	m_BtnEnchant->SetIsEnabled(HasItem);
 }

void UWidgetItemInfo::UpdateRegisterBtn()
{
	m_BtnRegister->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bool HasItem = false;

	if (UMyLib::IsEquip(m_ItemKey))
	{
		int RequireLevel = UItemCollectionTable::GetItemCollecTable->FindRow<FItemCollecRow>(m_CollecID,"")->m_AryItems[m_nCollecIndex].m_nEnchantLv;
		
		HasItem = UMyLib::FindEquipItem(m_ItemKey,RequireLevel) != nullptr;
	}
	else
	{
		HasItem = UMyLib::FindMiscItem(m_ItemKey) != nullptr;
	}
	m_BtnRegister->SetIsEnabled(HasItem);
}

void UWidgetItemInfo::OnEnchant()
{
	UMyLib::GetCanvas()->OpenEnchant();

	UInventory *Inven =  UMyLib::FindEquipItem(m_ItemKey,0);
	
	UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(m_ItemKey,Inven);

	OnClose();
}

void UWidgetItemInfo::OnRegister()
{
	UMyGameInstance::Get->m_ItemCollecManager->AddItem(m_CollecID,m_nCollecIndex,m_Inven.Get());
}

void UWidgetItemInfo::UpdateStat(const FName& target, int level)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(target);

	const FStatGroup TotalStat = (ItemData.m_EnchantStats * level) + ItemData.m_EquipStats;

	if(TotalStat.m_Dmg > 0)
	{
		CreateOption(TEXT("데미지"), TEXT("+{0}"), TotalStat.m_Dmg);
	}
	if(TotalStat.m_nAccu > 0)
	{
		CreateOption(TEXT("명중"), TEXT("+{0}"), TotalStat.m_nAccu);
	}
	if(TotalStat.m_AtkSpeed > 0)
	{
		CreateOption(TEXT("공격속도"), TEXT("{0}%"), TotalStat.m_AtkSpeed);
	}
	if(TotalStat.m_MaxHp > 0)
	{
		CreateOption(TEXT("체력"), TEXT("+{0}"), TotalStat.m_MaxHp);
	}
	if(TotalStat.m_nAvoid > 0)
	{
		CreateOption(TEXT("회피"), TEXT("+{0}"), TotalStat.m_nAvoid);
	}
	if(TotalStat.m_DmgReduce > 0)
	{
		CreateOption(TEXT("데미지 리덕션"), TEXT("+{0}"), TotalStat.m_DmgReduce);
	}
	if(TotalStat.m_CriPer > 0)
	{
		CreateOption(TEXT("치명 확률"), TEXT("{0}%"), TotalStat.m_CriPer);
	}
	if(TotalStat.m_CriDmg > 0)
	{
		CreateOption(TEXT("치명 데미지"), TEXT("{0}%"), TotalStat.m_CriDmg);
	}

	if (level > 0)
	{
		for (const auto& ClassOp : ItemData.m_Options)
		{
			CreateOption(ClassOp, level);
		}
	}
}

void UWidgetItemInfo::CreateOption(const FString&& infoText, const FString&& formatText, int v)
{
	UWidgetCollecStatChild* WidgetOp =  CreateWidget<UWidgetCollecStatChild>(this, m_ClassOption);

	m_ScrollInfo->AddChild(WidgetOp);

	m_AryOptions.Add(WidgetOp);

	WidgetOp->SetStatText(*infoText,*formatText,v);
}

void UWidgetItemInfo::CreateOption(TSubclassOf<UOptionBase> op, int lv)
{
	UOptionBase* OpClass = op->GetDefaultObject<UOptionBase>();
	
	CreateOption(OpClass->GetOptionName(), OpClass->GetOptionFormat(), OpClass->GetEnchantValue(lv));
}
