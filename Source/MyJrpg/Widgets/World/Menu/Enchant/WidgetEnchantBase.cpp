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
	int Level = UMyLib::GetPlayerInven()->GetItemLevel(TargetEquip);
	//주스텟 보조스텟 업그레이드, 룸티스 장신구처럼 많은건 어떻게 해야할까?
	
	FString AfterLevelStr  = FString::Printf(TEXT("+%d"),Level + 1);
	
	FString BeforeLevelStr  = FString::Printf(TEXT("+%d"),Level);
	
	m_TextLevelBefore->SetText(FText::FromString(BeforeLevelStr));

	m_TextLevelAfter->SetText(FText::FromString(AfterLevelStr));
}

void UWidgetEnchantBase::UpdateEnchantBtn(UEnchantManager* Enchant)
{
	m_BtnEnchant->SetIsEnabled(Enchant->IsEnchantAvailable());

	int Cost = Enchant->GetEnchantCost();

	m_TextEnchantCost->SetText(FText::AsNumber(Cost));
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
