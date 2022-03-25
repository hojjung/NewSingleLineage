#include "WidgetEnchantBase.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Equipment/WidgetEquipInvenPanel.h"

void UWidgetEnchantBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Inven->Init(UMyLib::GetPlayerInven(), EPanelType::Enchant);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetEnchantBase::OnClose);

	m_BtnEnchant->OnClicked.AddDynamic(this, &UWidgetEnchantBase::DoEnchant);

	UMyGameInstance::Get->m_EnchantManager->m_OnEnchantChanged.AddUObject(this,&UWidgetEnchantBase::Update);

	m_TargetItem->Init(EPanelType::None,UMyLib::GetPlayerInven());

	m_TargetMaterial->Init(EPanelType::None,UMyLib::GetPlayerInven());
}

void UWidgetEnchantBase::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Inven->OpenPanel();

	//Update();
}

void UWidgetEnchantBase::SetEnchantEquipTarget(const FName& target)
{
	UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(target);
}

void UWidgetEnchantBase::SetEnchantEquipMaterial(const FName& mat)
{
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(mat);
}

void UWidgetEnchantBase::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	m_Inven->ClosePanel();

	UMyGameInstance::Get->m_EnchantManager->Clear();
}

void UWidgetEnchantBase::DoEnchant()
{
	UMyGameInstance::Get->m_EnchantManager->DoEnchant();
}

void UWidgetEnchantBase::UpdateIcons(const FName& target, const FName& mat, int level)
{
	if (!target.IsNone())
	{
		m_TargetItem->UpdateElement(target);
	}
	else
	{
		m_TargetItem->Clear();
	}

	if (!mat.IsNone())
	{
		m_TargetMaterial->UpdateElement(mat);
	}
	else
	{
		m_TargetMaterial->Clear();
	}
}

void UWidgetEnchantBase::UpdateInfoTexts(const UEnchantManager* Enchant, const FName& target, const FName& mat, int level)
{
	if (target.IsNone() || mat.IsNone())
	{
		m_BarEnchantLevel->SetPercent(0);
		
		m_TextInfo->SetVisibility(ESlateVisibility::Collapsed);
		
		return;
	}
	
	m_TextInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FString AfterLevelStr  = FString::Printf(TEXT("+%d"),level + 1);
	
	m_TextLevel->SetText(FText::FromString(AfterLevelStr));
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

void UWidgetEnchantBase::UpdateBeforeAfter(const FName& target, int level)
{
	for(UWidgetEnchantOption* Op : m_AryOptions)
	{
		Op->RemoveFromParent();
	}
	
	m_AryOptions.Reset();
	
	if(target.IsNone())
	{
		m_StatLevel->SetVisibility(ESlateVisibility::Collapsed);
		
		return;
	}

	m_StatLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_StatLevel->SetBeforeAfter(TEXT("강화레벨"),TEXT("{0}"),level, level + 1);

	const FItemDataRow& ItemData = UMyLib::GetItemData(target);
	
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
}

void UWidgetEnchantBase::UpdateEnchantBtn(const UEnchantManager* Enchant)
{
	m_BtnEnchant->SetIsEnabled(Enchant->IsEnchantAvailable());

	int Cost = Enchant->GetEnchantCost();

	m_TextEnchantCost->SetText(FText::AsNumber(Cost));
}

void UWidgetEnchantBase::CreateOption(const FString&& infoText, const FString&& formatText, int beforeValue, int afterValue)
{
	UWidgetEnchantOption* WidgetOp =  CreateWidget<UWidgetEnchantOption>(this, m_ClassOption);

	m_ScrollInfo->AddChild(WidgetOp);

	m_AryOptions.Add(WidgetOp);

	WidgetOp->SetBeforeAfter(*infoText,*formatText,beforeValue,afterValue);
}

void UWidgetEnchantBase::Update()
{
	UEnchantManager* Enchant = UMyGameInstance::Get->m_EnchantManager;

	const FName& Target = Enchant->GetCrntTarget();

	const FName& Mat = Enchant->GetCrntMat();

	int Level = Enchant->GetCrntLevel();
	
	UpdateIcons(Target,Mat,Level);
	//
	UpdateInfoTexts(Enchant,Target,Mat,Level);
	//
	UpdateBeforeAfter(Target,Level);
	//
	UpdateEnchantBtn(Enchant);
}

