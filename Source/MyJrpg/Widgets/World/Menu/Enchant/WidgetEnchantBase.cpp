#include "WidgetEnchantBase.h"

#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Equipment/WidgetEquipInvenPanel.h"

void UWidgetEnchantBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetEnchantBase::OnClose);

	m_BtnEnchant->OnClicked.AddDynamic(this, &UWidgetEnchantBase::DoEnchant);

	m_TargetItem->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_TargetMaterial->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	UMyGameInstance::Get->m_EnchantManager->m_OnEnchantChanged.AddUObject(this,&UWidgetEnchantBase::Update);

	m_InvenPanel->Init(UMyLib::GetPlayerInven());

	m_InvenPanel->SetHoldable(false);

	m_InvenPanel->m_OnFocus.AddUObject(this, &UWidgetEnchantBase::OnInvenFocused);
}

void UWidgetEnchantBase::UpdateInvens()
{
	m_InvenPanel->OpenPanel();
	//
	if(!UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_BagPanel->Clear();
	}
	else
	{
		m_BagPanel->Init(UMyGameInstance::Get->m_EquipManager->GetBag());
		m_BagPanel->SetHoldable(false);
		m_BagPanel->OpenPanel();
	}

	if(!UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_BeltPanel->Clear();
	}
	else
	{
		m_BeltPanel->Init(UMyGameInstance::Get->m_EquipManager->GetBelt());
		m_BeltPanel->SetHoldable(false);
		m_BeltPanel->OpenPanel();
	}
}

void UWidgetEnchantBase::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Update();
}

void UWidgetEnchantBase::SetEnchantEquipTarget(FItemSpec& target, UInventory* inven)
{
	UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(target, inven);
}

void UWidgetEnchantBase::SetEnchantEquipMaterial(FItemSpec& mat, UInventory* inven)
{
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(mat, inven);
}

void UWidgetEnchantBase::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	m_InvenPanel->ClosePanel();

	m_BagPanel->ClosePanel();

	m_BeltPanel->ClosePanel();

	UMyGameInstance::Get->m_EnchantManager->Clear();
}

void UWidgetEnchantBase::DoEnchant()
{
	UMyGameInstance::Get->m_EnchantManager->DoEnchant();
}

void UWidgetEnchantBase::UpdateIcons(const FItemSpec* target, const FItemSpec* mat)
{
	if (target)
	{
		m_TargetItem->SetItem(*target);
	}
	else
	{
		m_TargetItem->Clear();
	}

	if (mat)
	{
		m_TargetMaterial->SetItem(*mat);
	}
	else
	{
		m_TargetMaterial->Clear();
	}
}

void UWidgetEnchantBase::UpdateInfoTexts(const UEnchantManager* Enchant, const FItemSpec* target, const FItemSpec* mat, int level)
{
	FString AfterLevelStr  = FString::Printf(TEXT("+%d"),level + 1);
	
	m_TextLevel->SetText(FText::FromString(AfterLevelStr));
	
	if (!target || !mat)
	{
		m_BarEnchantLevel->SetPercent(0);
		
		m_TextInfo->SetVisibility(ESlateVisibility::Hidden);

		if (!target)
			m_TextLevel->SetVisibility(ESlateVisibility::Hidden);
		else
			m_TextLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		return;
	}

	m_TextLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//
	float Percent = (float)level / (float)FGlobalVariable::ENCHANT_MAX;

	m_BarEnchantLevel->SetPercent(Percent);
	//
	float EnchantPer = Enchant->GetEnchantPercent();

	if (EnchantPer < 1.f)
	{
		m_TextInfo->SetColorAndOpacity(FLinearColor::Red);
	}
	else
	{
		m_TextInfo->SetColorAndOpacity(FLinearColor::White);
	}
	FString InfoStr = FString::Printf(TEXT("성공확률 %2.0f%% %s"), EnchantPer * 100.f, EnchantPer < 1.f ? TEXT("실패시 장비 파괴!"):TEXT(""));
	
	m_TextInfo->SetText(FText::FromString(InfoStr));
}

void UWidgetEnchantBase::UpdateBeforeAfter(const FItemSpec* target, int level)
{
	for(UWidgetEnchantOption* Op : m_AryOptions)
	{
		Op->RemoveFromParent();
	}
	
	m_AryOptions.Reset();
	
	if(!target)
	{
		m_StatLevel->SetVisibility(ESlateVisibility::Hidden);
		
		return;
	}

	m_StatLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_StatLevel->SetBeforeAfter(TEXT("강화레벨"),TEXT("{0}"),level, level + 1);

	const FItemDataRow& ItemData = UMyLib::GetItemData(target->m_ID);
	
	const auto& EnchantStat = ItemData.m_EnchantStats;

	FStatGroup BeforeStat = EnchantStat * level;

	FStatGroup AfterStat = EnchantStat * (level + 1);

	if(AfterStat.m_Dmg > 0)
	{
		CreateOption(TEXT("데미지"), TEXT("+{0}"), BeforeStat.m_Dmg ,AfterStat.m_Dmg);
	}
	if(AfterStat.m_nAccu > 0)
	{
		CreateOption(TEXT("명중"), TEXT("+{0}"), BeforeStat.m_nAccu ,AfterStat.m_nAccu);
	}
	if(AfterStat.m_AtkSpeed > 0)
	{
		CreateOption(TEXT("공격속도"), TEXT("{0}%"), BeforeStat.m_AtkSpeed ,AfterStat.m_AtkSpeed);
	}
	if(AfterStat.m_MaxHp > 0)
	{
		CreateOption(TEXT("체력"), TEXT("+{0}"), BeforeStat.m_MaxHp ,AfterStat.m_MaxHp);
	}
	if(AfterStat.m_nAvoid > 0)
	{
		CreateOption(TEXT("회피"), TEXT("+{0}"), BeforeStat.m_nAvoid ,AfterStat.m_nAvoid);
	}
	if(AfterStat.m_DmgReduce > 0)
	{
		CreateOption(TEXT("데미지 리덕션"), TEXT("+{0}"), BeforeStat.m_DmgReduce ,AfterStat.m_DmgReduce);
	}
	if(AfterStat.m_CriPer > 0)
	{
		CreateOption(TEXT("치명 확률"), TEXT("{0}%"), BeforeStat.m_CriPer ,AfterStat.m_CriPer);
	}
	if(AfterStat.m_CriDmg > 0)
	{
		CreateOption(TEXT("치명 데미지"), TEXT("{0}%"), BeforeStat.m_CriDmg ,AfterStat.m_CriDmg);
	}
	for(const auto& ClassOp: ItemData.m_Options)
	{
		CreateOption(ClassOp, level);
	}
}

void UWidgetEnchantBase::UpdateEnchantBtn(const UEnchantManager* Enchant)
{
	m_BtnEnchant->SetIsEnabled(Enchant->IsEnchantAvailable());
}

void UWidgetEnchantBase::CreateOption(const FString&& infoText, const FString&& formatText, int beforeValue, int afterValue)
{
	UWidgetEnchantOption* WidgetOp =  CreateWidget<UWidgetEnchantOption>(this, m_ClassOption);

	m_ScrollInfo->AddChild(WidgetOp);

	m_AryOptions.Add(WidgetOp);

	WidgetOp->SetBeforeAfter(*infoText,*formatText,beforeValue,afterValue);
}

void UWidgetEnchantBase::CreateOption(TSubclassOf<UOptionBase> op, int lv)
{
	UOptionBase* OpClass = op->GetDefaultObject<UOptionBase>();
	
	CreateOption(OpClass->GetOptionName(), OpClass->GetOptionFormat(), OpClass->GetEnchantValue(lv), OpClass->GetEnchantValue(lv + 1));
}

void UWidgetEnchantBase::Update()
{
	UEnchantManager* Enchant = UMyGameInstance::Get->m_EnchantManager;

	const FItemSpec* Target = Enchant->GetTargetItem();

	const FItemSpec* Mat = Enchant->GetTargetMat();

	int Level = Enchant->GetCrntLevel();
	
	UpdateIcons(Target,Mat);
	//
	UpdateInfoTexts(Enchant,Target,Mat,Level);
	//
	UpdateBeforeAfter(Target,Level);
	//
	UpdateEnchantBtn(Enchant);
	
	UpdateInvens();
}

void UWidgetEnchantBase::OnInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	ele->SetMyUnFocus();

	FItemSpec& ItemSpec = inven->GetItemRef(index);

	if(UMyLib::IsEquip(ItemSpec.m_ID))
	{
		UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(ItemSpec,inven);
		return ;
	}
	
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(ItemSpec,inven);
}