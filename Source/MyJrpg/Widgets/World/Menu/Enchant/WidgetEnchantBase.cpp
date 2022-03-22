#include "WidgetEnchantBase.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Equipment/WidgetEquipInvenPanel.h"

void UWidgetEnchantBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Inven->Init(UMyLib::GetPlayerInven(), EPanelType::Enchant);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetEnchantBase::OnClose);

	m_TargetItem->SetHoldable(false);
	m_TargetItem->SetFocusable(false);

	m_TargetMaterial->SetHoldable(false);
	m_TargetMaterial->SetFocusable(false);

	UMyGameInstance::Get->m_EnchantManager->m_OnEnchantChanged.AddUObject(this,&UWidgetEnchantBase::Update);
}

void UWidgetEnchantBase::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Update();
}

void UWidgetEnchantBase::SetEnchantEquipTarget(FItemSpec& target)
{
	UMyGameInstance::Get->m_EnchantManager->SetTargetEquip(target);
}

void UWidgetEnchantBase::SetEnchantEquipMaterial(FItemSpec& mat)
{
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(mat);
}

void UWidgetEnchantBase::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_EnchantManager->Clear();
}

void UWidgetEnchantBase::Update()
{
	UEnchantManager* Enchant = UMyGameInstance::Get->m_EnchantManager;
	
	FItemSpec* TargetEquip = Enchant->GetCrntTarget();

	if (TargetEquip)
	{
		const FItemDataRow& TargetFound = UMyLib::GetItemData(*TargetEquip);
		
		m_TargetItem->SetIcon(TargetFound.m_ItemIcon);

		m_TargetItem->SetGlowColor(TargetFound.m_ColorHandle);
	}
	else
	{
		m_TargetItem->Clear();
	}

	FItemSpec* EnchantMat = Enchant->GetCrntMat();

	if (EnchantMat)
	{
		const FItemDataRow& MatFound = UMyLib::GetItemData(*EnchantMat);
		
		m_TargetMaterial->SetIcon(MatFound.m_ItemIcon);

		m_TargetMaterial->SetGlowColor(MatFound.m_ColorHandle);
	}
	else
	{
		m_TargetMaterial->Clear();
	}

	
}
