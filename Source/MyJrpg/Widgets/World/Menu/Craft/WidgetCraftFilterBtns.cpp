#include "WidgetCraftFilterBtns.h"
#include "WidgetCraftPanel.h"
#include "MyJrpg/MyLib.h"

void UWidgetCraftFilterBtns::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnSelected = nullptr;
	m_EquipType	 = EEquipSlotType::None;
	m_BtnWeapon->IsFocusable = false;
	m_BtnHead->IsFocusable = false;
	m_BtnTorso->IsFocusable = false;
	m_BtnHand->IsFocusable = false;
	m_BtnLeg->IsFocusable = false;
	m_BtnRing->IsFocusable = false;
	m_BtnNeckless->IsFocusable = false;
	m_BtnEarRing->IsFocusable = false;
	m_BtnBelt->IsFocusable = false;

	UMyLib::SetBtnTint(m_BtnWeapon,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnHead,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnTorso,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnHand,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnLeg,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnRing,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnNeckless,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnEarRing,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnBelt,FLinearColor::Gray);
	//
	m_BtnWeapon->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterWeapon);
	m_BtnHead->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterHead);
	m_BtnTorso->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterTorso);
	m_BtnHand->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterHand);
	m_BtnLeg->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterLeg);
	m_BtnRing->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterRing);
	m_BtnNeckless->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterNeckless);
	m_BtnEarRing->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterEarRing);
	m_BtnBelt->OnClicked.AddDynamic(this, &UWidgetCraftFilterBtns::OnFilterBelt);
}

void UWidgetCraftFilterBtns::ClearFilter()
{
	m_EquipType	 = EEquipSlotType::None;
	
	if(m_BtnSelected)
		UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::Gray);
	
	m_BtnSelected = nullptr;
}

void UWidgetCraftFilterBtns::OnBtnPressed(UButton* btn ,EEquipSlotType type)
{
	if (m_BtnSelected == btn)
	{
		ClearFilter();
		return;
	}
	if(m_BtnSelected)
	{
		UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::Gray);
	}
	m_BtnSelected = btn;

	UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::White);
	
	m_EquipType = type;
}

void UWidgetCraftFilterBtns::RegisterFilter(UWidgetCraftPanel* craft)
{
	m_BtnWeapon->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnHead->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnTorso->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnHand->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnLeg->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnRing->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnNeckless->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnEarRing->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
	m_BtnBelt->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterEquip);
}

EEquipSlotType UWidgetCraftFilterBtns::GetCrntEquipType()
{
	return m_EquipType;
}

void UWidgetCraftFilterBtns::OnFilterWeapon()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Weapon);
}

void UWidgetCraftFilterBtns::OnFilterHead()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Head);
}

void UWidgetCraftFilterBtns::OnFilterTorso()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Torso);
}

void UWidgetCraftFilterBtns::OnFilterHand()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Head);
}

void UWidgetCraftFilterBtns::OnFilterLeg()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Leg);
}

void UWidgetCraftFilterBtns::OnFilterRing()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Ring);
}

void UWidgetCraftFilterBtns::OnFilterNeckless()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Neckless);
}

void UWidgetCraftFilterBtns::OnFilterEarRing()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::EarRing);
}

void UWidgetCraftFilterBtns::OnFilterBelt()
{
	OnBtnPressed(m_BtnWeapon, EEquipSlotType::Belt);
}

void UWidgetCraftColorFilterBtns::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_Color = NAME_None;
	m_BtnWhite->IsFocusable = false;
	m_BtnGreen->IsFocusable = false;
	m_BtnBlue->IsFocusable = false;
	m_BtnRed->IsFocusable = false;
	m_BtnPurple->IsFocusable = false;
	m_BtnYellow->IsFocusable = false;
	//
	UMyLib::SetBtnTint(m_BtnWhite,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnGreen,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnBlue,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnRed,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnPurple,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnYellow,FLinearColor::Gray);
	//
	m_BtnWhite->OnClicked.AddDynamic(this, &UWidgetCraftColorFilterBtns::OnFilterWhite);
	m_BtnGreen->OnClicked.AddDynamic(this, &UWidgetCraftColorFilterBtns::OnFilterGreen);
	m_BtnBlue->OnClicked.AddDynamic(this, &UWidgetCraftColorFilterBtns::OnFilterBlue);
	m_BtnRed->OnClicked.AddDynamic(this, &UWidgetCraftColorFilterBtns::OnFilterRed);
	m_BtnPurple->OnClicked.AddDynamic(this, &UWidgetCraftColorFilterBtns::OnFilterPurple);
	m_BtnYellow->OnClicked.AddDynamic(this, &UWidgetCraftColorFilterBtns::OnFilterYellow);
}

void UWidgetCraftColorFilterBtns::OnBtnPressed(UButton* btn, FName&& c)
{
	if (m_BtnSelected == btn)
	{
		ClearFilter();
		return;
	}
	if(m_BtnSelected)
	{
		UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::Gray);
	}
	m_BtnSelected = btn;

	UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::White);
	
	m_Color = c;
}

void UWidgetCraftColorFilterBtns::ClearFilter()
{
	m_Color = NAME_None;

	if(m_BtnSelected)
		UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::Gray);
	m_BtnSelected = nullptr;
}

void UWidgetCraftColorFilterBtns::RegisterFilter(UWidgetCraftPanel* craft)
{
	m_BtnWhite->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterColor);
	m_BtnGreen->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterColor);
	m_BtnBlue->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterColor);
	m_BtnRed->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterColor);
	m_BtnPurple->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterColor);
	m_BtnYellow->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterColor);
}

FName UWidgetCraftColorFilterBtns::GetCrntColor()
{
	return m_Color;
}

void UWidgetCraftColorFilterBtns::OnFilterWhite()
{
	OnBtnPressed(m_BtnWhite,TEXT("Default"));
}

void UWidgetCraftColorFilterBtns::OnFilterGreen()
{
	OnBtnPressed(m_BtnGreen,TEXT("Green"));
}

void UWidgetCraftColorFilterBtns::OnFilterBlue()
{
	OnBtnPressed(m_BtnBlue,TEXT("Blue"));
}

void UWidgetCraftColorFilterBtns::OnFilterRed()
{
	OnBtnPressed(m_BtnRed,TEXT("Red"));
}

void UWidgetCraftColorFilterBtns::OnFilterPurple()
{
	OnBtnPressed(m_BtnPurple,TEXT("Purple"));
}

void UWidgetCraftColorFilterBtns::OnFilterYellow()
{
	OnBtnPressed(m_BtnYellow,TEXT("Yellow"));
}
