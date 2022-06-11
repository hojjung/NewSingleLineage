#include "WidgetInteract.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UWidgetInteract::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bAutoToggle = false;

	m_bHasFocus = false;

	m_BtnInteract->OnClicked.AddDynamic(this, &UWidgetInteract::OnInteract);
	m_BtnAttack->OnClicked.AddDynamic(this, &UWidgetInteract::OnAttack);
	m_BtnSneak->OnClicked.AddDynamic(this, &UWidgetInteract::OnSneak);
	m_BtnAuto->OnClicked.AddDynamic(this, &UWidgetInteract::OnAutoToggle);
	
	m_BtnInteract->SetVisibility(ESlateVisibility::Collapsed);

	m_Pl = UMyLib::GetPlayer();
	m_Pl->m_OnFocus.AddUObject(this, &UWidgetInteract::ShowInteract);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetInteract::OnEquipChanged);
	UMyGameInstance::Get->m_EquipManager->m_OnDurChanged.AddUObject(this, &UWidgetInteract::OnEquipChanged);
	
	OnEquipChanged();

	m_ImgUseAuto->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetInteract::HideDur()
{
	m_DurGauge->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetInteract::ShowDur(float per)
{
	m_DurGauge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_DurGauge->SetProgressValue(per);
}

void UWidgetInteract::ShowInteract(IFocusable* focus)
{
	m_BtnInteract->SetVisibility(ESlateVisibility::Collapsed);
	
	m_bHasFocus = true;
	
	AMonsterPawn* Monster = Cast<AMonsterPawn>(focus);

	if(!focus || (Monster && Monster->IsAlive()))
	{
		return;
	}
	
	m_BtnInteract->SetVisibility(ESlateVisibility::Visible);

	FText InterText = focus->GetTextInteract();
	
	m_TextInteract->SetText(InterText);
}

void UWidgetInteract::OnInteract()
{
	if(m_Pl->GetInteracting())
	{
		return;
	}
	IFocusable* Prop = m_Pl->GetFocusedTarget<IFocusable>();

	if(!Prop)
	{
		return;
	}
	Prop->OnInteract();
}

void UWidgetInteract::OnAttack()
{
	m_Pl->RequestAttack();
}

void UWidgetInteract::OnSneak()
{
	m_Pl->SetSneak();
}

void UWidgetInteract::OnAutoToggle()
{
	m_bAutoToggle = !m_bAutoToggle;

	if(m_bAutoToggle)
	{
		m_ImgUseAuto->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_ImgUseAuto->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	UMyLib::GetPlayer()->SetAutoCombat(m_bAutoToggle);
	
	UMyGameInstance::Get->m_SkillAuto->SetUseAuto(m_bAutoToggle);
}

void UWidgetInteract::OnEquipChanged()
{
	FItemSpec& Item = UMyGameInstance::Get->m_EquipManager->GetEquipItem(EEquipSlotType::Weapon);
	if(Item.m_ID.IsNone())
	{
		HideDur();
		return;
	}
	int Dur = (float)UMyLib::GetItemData(Item.m_ID).m_nDurability;
	float Per = (float)Item.m_nDurability / (float)Dur;
	ShowDur(Per);
}
