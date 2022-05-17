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

	m_BtnObtain->OnClicked.AddDynamic(this, &UWidgetInteract::OnObtain);
	m_BtnSteal->OnClicked.AddDynamic(this, &UWidgetInteract::OnSteal);
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
	if(!IsInRange(focus))
	{
		if(m_bHasFocus)
		{
			HideAllBtns();
		}
		m_bHasFocus = false;
		return;
	}
	m_bHasFocus = true;
	
	m_Focused.SetInterface(focus);
	
	AMonsterPawn* Monster = Cast<AMonsterPawn>(focus);
	if(Monster)
	{
		m_Focused.SetObject(Monster);
		ShowWidgetMonster(Monster);
		return;
	}
	
	AItemActor* Item = Cast<AItemActor>(focus);
	if(Item)
	{
		m_Focused.SetObject(Item);
		ShowWidgetItem(Item);
		return;
	}
	
	AActor* Prop = Cast<AActor>(focus);
	if(Prop)
	{
		m_Focused.SetObject(Prop);
		ShowWidgetProp();
		return;
	}
}

void UWidgetInteract::OnObtain()
{
	if(!m_Focused.GetObject())
	{
		return;
	}
	AItemActor* Item = Cast<AItemActor>(m_Focused.GetObject());
	
	Item->Obtain();

	m_Focused.SetObject(nullptr);
	m_Focused.SetInterface(nullptr);
}

void UWidgetInteract::OnSteal()
{
	OnObtain();
}

void UWidgetInteract::OnControl()
{
	IFocusable* Prop = Cast<IFocusable>(m_Focused.GetObject());
	
	Prop->OnInteract();
}

void UWidgetInteract::OnTalk()
{
	AMonsterPawn* Mob = Cast<AMonsterPawn>(m_Focused.GetObject());
	
	const FName& TalkID = Mob->GetTalkID();

	if (!TalkID.IsNone())
		UMyLib::GetCanvas()->StartDialogue(TalkID);
}

void UWidgetInteract::OnAttack()
{
	AMonsterPawn* Mob = Cast<AMonsterPawn>(m_Focused.GetObject());
	
	m_Pl->SetFocusedTarget(Mob);
	
	m_Pl->TryAttack();
}

void UWidgetInteract::OnPickPocket()
{
	AMonsterPawn* Mob = Cast<AMonsterPawn>(m_Focused.GetObject());
	
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
