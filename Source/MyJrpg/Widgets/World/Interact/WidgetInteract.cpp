#include "WidgetInteract.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

bool UWidgetInteract::AutoToggle = false;

void UWidgetInteract::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bHasFocus = false;

	m_BtnObtain->OnClicked.AddDynamic(this, &UWidgetInteract::OnControl);
	m_BtnSteal->OnClicked.AddDynamic(this, &UWidgetInteract::OnControl);
	m_BtnControl->OnClicked.AddDynamic(this, &UWidgetInteract::OnControl);
	m_BtnTalk->OnClicked.AddDynamic(this, &UWidgetInteract::OnTalk);
	m_BtnAttack->OnClicked.AddDynamic(this, &UWidgetInteract::OnAttack);
	m_BtnPickPocket->OnClicked.AddDynamic(this, &UWidgetInteract::OnPickPocket);
	m_BtnSneak->OnClicked.AddDynamic(this, &UWidgetInteract::OnSneak);
	m_BtnAuto->OnClicked.AddDynamic(this, &UWidgetInteract::OnAutoToggle);

	m_BtnObtain->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnSteal->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnControl->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnTalk->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnPickPocket->SetVisibility(ESlateVisibility::Collapsed);

	m_Pl = UMyLib::GetPlayer();
	m_Pl->m_OnFocus.AddUObject(this, &UWidgetInteract::ShowInteract);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetInteract::OnEquipChanged);
	UMyGameInstance::Get->m_EquipManager->m_OnDurChanged.AddUObject(this, &UWidgetInteract::OnEquipChanged);
	OnEquipChanged();
}

void UWidgetInteract::ShowWidgetMonster(const AMonsterPawn* mob)
{
	m_BtnObtain->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnSteal->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnControl->SetVisibility(ESlateVisibility::Collapsed);

	if(mob->GetFocusedTarget() != m_Pl)
	{
		m_BtnPickPocket->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if(!UMyGameInstance::Get->m_TeamKarma->IsFoe(mob))
	{
		m_BtnTalk->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetInteract::ShowWidgetItem(const AItemActor* item)
{
	m_BtnControl->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnPickPocket->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnTalk->SetVisibility(ESlateVisibility::Collapsed);
	
	if(item->HasOwnerTeamID())
	{
		m_BtnSteal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		return;
	}
	m_BtnObtain->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetInteract::ShowWidgetProp()
{
	m_BtnPickPocket->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnTalk->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnSteal->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnObtain->SetVisibility(ESlateVisibility::Collapsed);
	
	m_BtnControl->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetInteract::HideAllBtns()
{
	m_BtnPickPocket->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnTalk->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnSteal->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnObtain->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnControl->SetVisibility(ESlateVisibility::Collapsed);
}

bool UWidgetInteract::IsInRange(IFocusable* focus)
{
	if(!focus)
		return false;
	
	AActor* FocusActor = Cast<AActor>(focus);

	FVector PlayerLoc = UMyLib::GetPlayer()->GetActorLocation();

	FVector FocusLoc = FocusActor->GetActorLocation();

	return FVector::DistSquared2D(PlayerLoc, FocusLoc) <= 90000;
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
	// if(!IsInRange(focus))
	// {
	// 	if(m_bHasFocus)
	// 	{
	// 		HideAllBtns();
	// 	}
	// 	m_bHasFocus = false;
	// 	return;
	// }
	m_bHasFocus = true;
	
	AMonsterPawn* Monster = Cast<AMonsterPawn>(focus);
	if(Monster)
	{
		ShowWidgetMonster(Monster);
		return;
	}
	
	AItemActor* Item = Cast<AItemActor>(focus);
	if(Item)
	{
		ShowWidgetItem(Item);
		return;
	}
	
	AActor* Prop = Cast<AActor>(focus);
	if(Prop)
	{
		ShowWidgetProp();
		return;
	}
}

void UWidgetInteract::OnControl()
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

void UWidgetInteract::OnTalk()
{
	AMonsterPawn* Prop = m_Pl->GetFocusedTarget<AMonsterPawn>();
	
	const FName& TalkID = Prop->GetTalkID();

	if (!TalkID.IsNone())
		UMyLib::GetCanvas()->StartDialogue(TalkID);
}

void UWidgetInteract::OnAttack()
{
	m_Pl->RequestAttack();
}

void UWidgetInteract::OnPickPocket()
{
	AMonsterPawn* Mob = m_Pl->GetFocusedTarget<AMonsterPawn>();
	
	UMyLib::GetCanvas()->StartPickPocket(Mob);
}

void UWidgetInteract::OnSneak()
{
	m_Pl->SetSneak();
}

void UWidgetInteract::OnAutoToggle()
{
	UWidgetInteract::AutoToggle=!UWidgetInteract::AutoToggle;
	UMyLib::GetPlayer()->SetAutoCombat(AutoToggle);
	UMyGameInstance::Get->m_SkillAuto->SetUseAuto(AutoToggle);
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