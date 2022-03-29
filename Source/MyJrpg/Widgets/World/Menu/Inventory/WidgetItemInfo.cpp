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
	
	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnClose);

	m_BtnEraseItem->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnErase);

	m_BtnEnchant->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnEnchant);
}

void UWidgetItemInfo::SetTypeInfo(EItemInfo info, EItemType type, const FItemDataRow& ItemData)
{
	
}

void UWidgetItemInfo::SetItemInfo(EItemInfo info,const FName& oID,UInventory* inven)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	for(UWidgetCollecStatChild* Op : m_AryOptions)
	{
		Op->RemoveFromParent();
	}
	
	m_AryOptions.Reset();
	
	m_ItemKey = oID;

	m_ItemIcon->Init(EPanelType::Inven,inven);

	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemKey);

	EItemType Type = UMyLib::GetItemType(ItemData);

	m_ItemIcon->UpdateElement(m_ItemKey);
	
	m_TextItemName->SetText(ItemData.m_TextShowingName);

	m_TextItemDesc->SetText(ItemData.m_TextDesc);

	if (Type == EItemType::Equip)
	{
		int Lv = inven ? inven->GetItemLevel(oID) : 0;
		
		m_BtnEraseItem->SetIsEnabled(!UMyLib::GetEquip()->IsItemEquipped(m_ItemKey));
		
		UpdateStat(m_ItemKey,Lv);
	}
	
	SetTypeInfo(info,Type,ItemData);
}

void UWidgetItemInfo::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetItemInfo::OnErase()
{
	//패널에서 버릴 숫자를 정할수 있어야한다.
	//if(m_EquipItem)
	{
	//	UMyLib::GetPlayerInven()->RemoveItem(*m_EquipItem);
	}
	//else if(m_ItemKey!=NAME_None)
	{
		UMyLib::GetPlayerInven()->RemoveItem(m_ItemKey,m_nEraseAmount);
	}

	OnClose();
}

void UWidgetItemInfo::OnEnchant()
{
	
}

void UWidgetItemInfo::OnOpenCalculator()
{
	UMyLib::GetCanvas()->OpenCalculator(0);
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
