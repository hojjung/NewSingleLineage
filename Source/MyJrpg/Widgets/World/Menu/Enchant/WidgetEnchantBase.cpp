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

	Update();
}

void UWidgetEnchantBase::SetEnchantEquipTarget(FName& target)
{
	UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(target);
}

void UWidgetEnchantBase::SetEnchantEquipMaterial(FName& mat)
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

void UWidgetEnchantBase::UpdateIcons(const UEnchantManager* Enchant)
{
	FName TargetEquip = Enchant->GetCrntTarget();
	
	if (!TargetEquip.IsNone())
	{
		m_TargetItem->UpdateElement(TargetEquip);
	}
	else
	{
		m_TargetItem->Clear();
	}

	FName EnchantMat = Enchant->GetCrntMat();

	if (!EnchantMat.IsNone())
	{
		m_TargetMaterial->UpdateElement(EnchantMat);
	}
	else
	{
		m_TargetMaterial->Clear();
	}
}

void UWidgetEnchantBase::UpdateInfoTexts(UEnchantManager* Enchant)
{
	FName TargetEquip = Enchant->GetCrntTarget();

	FName TargetMat = Enchant->GetCrntMat();
	
	if (TargetEquip.IsNone() || TargetMat.IsNone())
	{
		return;
	}
	int Level = UMyLib::GetPlayerInven()->GetItemLevel(TargetEquip);
	
	FString AfterLevelStr  = FString::Printf(TEXT("+%d"),Level + 1);
	
	m_TextLevel->SetText(FText::FromString(AfterLevelStr));
	//
	float Percent = Level / FGlobalVariable::ENCHANT_MAX;

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

void UWidgetEnchantBase::UpdateBeforeAfter(const UEnchantManager* Enchant)
{
	FName TargetEquip = Enchant->GetCrntTarget();
	
	if(TargetEquip.IsNone())
	{
		return;
	}
	
	for(UWidgetEnchantOption* Op : m_AryOptions)
	{
		Op->RemoveFromParent();
	}
	m_AryOptions.Reset();
	
	int Level = UMyLib::GetPlayerInven()->GetItemLevel(TargetEquip);

	m_StatLevel->SetBeforeAfter(TEXT("강화레벨"),TEXT("{0}"),Level, Level + 1);

	const FItemDataRow& ItemData = UMyLib::GetItemData(TargetEquip);
	
	const auto& AryStats = ItemData.m_AryEnchantStats;

	FStatGroup PreStat = Level == 0 ? FStatGroup(0) : AryStats[Level - 1];

	if(AryStats[Level].m_Dmg > 0)
	{
		CreateOption(TEXT("데미지"), TEXT("+{0}"), PreStat.m_Dmg ,AryStats[Level].m_Dmg);
	}
	if(AryStats[Level].m_nAccu > 0)
	{
		CreateOption(TEXT("명중"), TEXT("+{0}"), PreStat.m_nAccu ,AryStats[Level].m_nAccu);
	}
	if(AryStats[Level].m_AtkSpeed > 0)
	{
		CreateOption(TEXT("공격속도"), TEXT("{0}%"), PreStat.m_AtkSpeed ,AryStats[Level].m_AtkSpeed);
	}
	if(AryStats[Level].m_MaxHp > 0)
	{
		CreateOption(TEXT("체력"), TEXT("+{0}"), PreStat.m_MaxHp ,AryStats[Level].m_MaxHp);
	}
	if(AryStats[Level].m_nAvoid > 0)
	{
		CreateOption(TEXT("회피"), TEXT("+{0}"), PreStat.m_nAvoid ,AryStats[Level].m_nAvoid);
	}
	if(AryStats[Level].m_DmgReduce > 0)
	{
		CreateOption(TEXT("데미지 리덕션"), TEXT("+{0}"), PreStat.m_DmgReduce ,AryStats[Level].m_DmgReduce);
	}
	if(AryStats[Level].m_CriPer > 0)
	{
		CreateOption(TEXT("치명 확률"), TEXT("{0}%"), PreStat.m_CriPer ,AryStats[Level].m_CriPer);
	}
	if(AryStats[Level].m_CriDmg > 0)
	{
		CreateOption(TEXT("치명 데미지"), TEXT("{0}%"), PreStat.m_CriDmg ,AryStats[Level].m_CriDmg);
	}
}

void UWidgetEnchantBase::UpdateEnchantBtn(UEnchantManager* Enchant)
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
	
	UpdateIcons(Enchant);
	//
	UpdateInfoTexts(Enchant);
	//
	UpdateBeforeAfter(Enchant);
	//
	UpdateEnchantBtn(Enchant);
}
