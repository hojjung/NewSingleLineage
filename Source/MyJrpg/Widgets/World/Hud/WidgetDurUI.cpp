#include "WidgetDurUI.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetDurUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_EquipManager->m_OnDurChanged.AddUObject(this, &UWidgetDurUI::UpdateDurUI);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetDurUI::UpdateDurUI);

	UpdateDurUI();
}

void UWidgetDurUI::UpdateDurUI()
{
	int Iter = 0;

	bool NeedVisible = false;

	while (++Iter < (int)EEquipSlotType::Ring)
	{
		const FItemSpec& EquippedItem = UMyGameInstance::Get->m_EquipManager->GetEquipItem(Iter);

		float Dur = EquippedItem.m_nDurability;
		
		if(EquippedItem.m_ID.IsNone() || Dur >= 12)
		{
			SetColorIcon((EEquipSlotType)Iter, FLinearColor::Gray);
			
		}
		else
		{
			SetColorIcon((EEquipSlotType)Iter, FLinearColor::Red);
			NeedVisible = true;
		}
	}

	if(NeedVisible)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidgetDurUI::SetColorIcon(EEquipSlotType t, FLinearColor c)
{
	UImage* Icon = nullptr;
	
	switch (t)
	{
	case EEquipSlotType::Weapon:
		Icon = m_ImgSword;
		break;
	case EEquipSlotType::Head:
		Icon = m_ImgHelmet;
		break;
	case EEquipSlotType::Torso:
		Icon = m_ImgTorso;
		break;
	case EEquipSlotType::Hand:
		Icon = m_ImgGauntlet;
		break;
	case EEquipSlotType::Leg:
		Icon = m_ImgLeg;
		break;
	}
	Icon->SetBrushTintColor(c);
}
