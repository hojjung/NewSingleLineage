// All right Reserve 2021 HereticByte


#include "WidgetItemInfo.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetConfirmPanel.h"

void UWidgetItemInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_ItemSpec = nullptr;

	m_nEraseAmount = 1;

	m_ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_ItemIcon->Clear();
	
	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnClose);

	m_BtnEraseItem->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnErase);

	m_BtnEnchant->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnEnchant);

	m_BtnRegister->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnRegister);

	m_BtnSplit->OnClicked.AddDynamic(this,&UWidgetItemInfo::OnSplit);
}

void UWidgetItemInfo::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	m_CollecID = NAME_None;

	m_CollecTargetItemID = NAME_None;
	
	m_nCollecIndex = INDEX_NONE;
}

void UWidgetItemInfo::OnErase()
{
	UWidgetConfirmPanel::FOnClick Cancel;

	UWidgetConfirmPanel::FOnClick Confirm = UWidgetConfirmPanel::FOnClick::CreateUObject(this,&UWidgetItemInfo::EraseConfirm); 

	FString DescStrF = NSLOCTEXT("UWidgetItemInfo","OnErase","정말로 {0} {1}개를 버릴까요?").ToString();

	int Count = UMyLib::IsEquip(m_ItemSpec->m_ID) ? 1 : m_ItemSpec->m_nLvStack;

	FString DescStr = FString::Format(*DescStrF, {*UMyLib::GetItemData(m_ItemSpec->m_ID).m_ShowingName.ToString(), Count});

	UMyLib::GetCanvas()->GetConfirmPanel()->SetConfirmPanel(DescStr,Cancel,Confirm);
}

void UWidgetItemInfo::EraseConfirm()
{
	m_Inven->RemoveItem(*m_ItemSpec);
	OnClose();
}

int UWidgetItemInfo::GetMax()
{
	return m_ItemSpec->m_nLvStack - 1;
}

void UWidgetItemInfo::SplitConfirm(int am)
{
	FItemSpec NewItem = *m_ItemSpec;

	NewItem.m_nLvStack = am;
	
	int EmptySlot;
	
	UMyLib::GetPlayerInven()->GetEmptyIndex(EmptySlot);

	UMyLib::GetPlayerInven()->AddSlot(EmptySlot, NewItem);
	
	UMyLib::GetPlayerInven()->AddItemKey(UMyLib::GetItemData(NewItem.m_ID),NewItem.m_ID,EmptySlot);

	m_ItemSpec->m_nLvStack -= am;
	
	UMyLib::GetPlayerInven()->UpdateInventory();
}


void UWidgetItemInfo::UpdateEnchantBtn()//가지고있으면 해당 인벤으로
{
	if(!m_Inven.Get())
	{
		return ;
	}
	if(!UMyLib::IsEquip(m_ItemSpec->m_ID))
	{
		m_BtnEnchant->SetIsEnabled(false);
		
		return ;
	}
	m_BtnEnchant->SetVisibility(ESlateVisibility::Visible);

	EEquipSlotType T = UMyLib::GetItemData(m_ItemSpec->m_ID).m_ItemType;
	
	if (&UMyLib::GetEquip()->GetEquipItem(T) == m_ItemSpec)
	{
		m_BtnEnchant->SetIsEnabled(false);
	
		return ;
	}

	m_BtnEnchant->SetIsEnabled(true);
}

void UWidgetItemInfo::UpdateSplitBtn()
{
	if(!m_Inven.Get())
	{
		return ;
	}
	if (UMyLib::IsEquip(m_ItemSpec->m_ID))
	{
		m_BtnSplit->SetVisibility(ESlateVisibility::Collapsed);
		return ;
	}
	m_BtnSplit->SetVisibility(ESlateVisibility::Visible);
	
	if (m_Inven->EmptySlotCount() < 1 || m_ItemSpec->m_nLvStack < 2)
	{
		m_BtnSplit->SetIsEnabled(false);
		return ;
	}
	m_BtnSplit->SetIsEnabled(true);
}


void UWidgetItemInfo::UpdateEraseBtn()
{
	if(m_Inven.Get())
	{
		m_BtnEraseItem->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWidgetItemInfo::SetItemInfo(FItemSpec& item, UInventory* inven)
{
	m_ItemSpec = &item;

	m_Inven = inven;
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemSpec->m_ID);
	
	SetInfoItemData(ItemData);

	UpdateStat(ItemData, m_ItemSpec->m_nLvStack);
	
	m_ItemIcon->SetItem(*m_ItemSpec);

	UpdateEraseBtn();
	UpdateEnchantBtn();
	UpdateSplitBtn();
}

void UWidgetItemInfo::SetInfoItemData(const FItemDataRow& data_row)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ItemIcon->SetItemData(data_row);

	m_TextItemName->SetText(data_row.m_ShowingName);

	m_BtnEraseItem->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnEnchant->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnRegister->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnSplit->SetVisibility(ESlateVisibility::Collapsed);

	EItemType t = UMyLib::GetItemType(data_row);

	if(EItemType::Equip == t)
	{
		m_VertItemOptions->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_TextItemEffectTitle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_VertItemOptions->SetVisibility(ESlateVisibility::Collapsed);
		m_TextItemEffectTitle->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	m_TextItemDesc->SetText(data_row.m_Desc);

	SetTypeText(t);
}

void UWidgetItemInfo::SetCollecItemInfo(const FName& collecId, int collecIndex, const FName& itemID)
{
	m_CollecID = collecId;

	m_nCollecIndex = collecIndex;
 
	m_CollecTargetItemID = itemID;
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(itemID);
	
	SetInfoItemData(ItemData);

	UpdateRegisterBtn();
}

void UWidgetItemInfo::SetTypeText(EItemType t)
{
	switch (t)
	{
	case EItemType::Consume:
		m_TextItemType->SetText(NSLOCTEXT("UWidgetItemInfo","TypeConsume","소모품"));
		break;
	case EItemType::Equip:
		m_TextItemType->SetText(NSLOCTEXT("UWidgetItemInfo","TypeEquip","장비"));
		break;
	}
	return m_TextItemType->SetText(NSLOCTEXT("UWidgetItemInfo","TypeMisc","재료"));
}


void UWidgetItemInfo::UpdateRegisterBtn()
{
	m_BtnRegister->SetVisibility(ESlateVisibility::Visible);

 	int RequireLevel = UMyLib::GetRequireCollecLevel(m_CollecID,m_nCollecIndex);
	 	
 	bool HasItem = UMyLib::FindItemAllInven(m_CollecTargetItemID,RequireLevel) != nullptr;
	
	m_BtnRegister->SetIsEnabled(HasItem);
}

void UWidgetItemInfo::OnEnchant()//강화가 두개의 상황이 존재함.그럼결국,콜렉션으로 열때 원본 아이템을 찾을수있어야함
{
	UMyLib::GetCanvas()->OpenEnchant();

	if(m_Inven.Get())
	{
		UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(*m_ItemSpec,m_Inven.Get());

		OnClose();
		return;
	}
}

void UWidgetItemInfo::OnRegister()
{
	UMyGameInstance::Get->m_ItemCollecManager->AddItem(m_CollecID,m_nCollecIndex);

	OnClose();
}

void UWidgetItemInfo::OnSplit()
{
	int Half = m_ItemSpec->m_nLvStack / 2;
	
	UWidgetStackCalculator* Calc = UMyLib::GetCanvas()->GetCalculator();
	
	Calc->m_OnGetMax.BindUObject(this, &UWidgetItemInfo::GetMax);
	
	Calc->m_OnNumberAccept.AddUObject(this,&UWidgetItemInfo::SplitConfirm);
	
	Calc->Open(Half);
	
	OnClose();
}

void UWidgetItemInfo::UpdateStat(const FItemDataRow& target, int level)
{
	const FStatGroup TotalStat = (target.m_EnchantStats * level) + target.m_EquipStats;

	if(TotalStat.m_Dmg > 0)
	{
		CreateOption(TEXT("데미지"), TEXT("+{0}"), TotalStat.m_Dmg);
	}
	if(TotalStat.m_nAccu > 0)
	{
		CreateOption(TEXT("명중"), TEXT("+{0}"), TotalStat.m_nAccu);
	}
	if(TotalStat.m_AtkPerSec > 0)
	{
		CreateOption(TEXT("공격속도"), TEXT("{0}%"), TotalStat.m_AtkPerSec);
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
		for (const auto& ClassOp : target.m_Options)
		{
			CreateOption(ClassOp, level);
		}
	}
}

void UWidgetItemInfo::CreateOption(const FString&& infoText, const FString&& formatText, int v)
{
	UWidgetCollecStatChild* WidgetOp =  CreateWidget<UWidgetCollecStatChild>(this, m_ClassOption);

	m_VertItemOptions->AddChild(WidgetOp);

	m_AryOptions.Add(WidgetOp);

	WidgetOp->SetStatText(*infoText,*formatText,v);
}

void UWidgetItemInfo::CreateOption(TSubclassOf<UOptionBase> op, int lv)
{
	UOptionBase* OpClass = op->GetDefaultObject<UOptionBase>();
	
	CreateOption(OpClass->GetOptionName(), OpClass->GetOptionFormat(), OpClass->GetEnchantValue(lv));
}
