#include "WidgetInteract.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/InteractActorBase.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UWidgetInteract::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnObtain->OnClicked.AddDynamic(this, &UWidgetInteract::OnObtain);
	m_BtnSteal->OnClicked.AddDynamic(this, &UWidgetInteract::OnSteal);
	m_BtnControl->OnClicked.AddDynamic(this, &UWidgetInteract::OnControl);
	m_BtnTalk->OnClicked.AddDynamic(this, &UWidgetInteract::OnTalk);
	m_BtnAttack->OnClicked.AddDynamic(this, &UWidgetInteract::OnAttack);
	m_BtnPickPocket->OnClicked.AddDynamic(this, &UWidgetInteract::OnPickPocket);
	m_BtnSneak->OnClicked.AddDynamic(this, &UWidgetInteract::OnSneak);
	m_BtnAuto->OnClicked.AddDynamic(this, &UWidgetInteract::AutoToggle);

	m_BtnObtain->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnSteal->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnControl->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnTalk->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnPickPocket->SetVisibility(ESlateVisibility::Collapsed);

	m_AutoToggle=false;
	
	m_Pl = UMyLib::GetPlayer();
	m_Pl->m_OnFocus.AddUObject(this, &UWidgetInteract::ShowInteract);
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

void UWidgetInteract::ShowInteract(IFocusable* focus)
{
	if(!focus)
	{
		HideAllBtns();
		return;
	}
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
	
	AInteractActorBase* Prop = Cast<AInteractActorBase>(focus);
	if(Prop)
	{
		m_Focused.SetObject(Prop);
		ShowWidgetProp();
		return;
	}
}

void UWidgetInteract::OnObtain()
{
	AItemActor* Item = Cast<AItemActor>(m_Focused.GetObject());
	
	Item->Obtain();
}

void UWidgetInteract::OnSteal()
{
	OnObtain();
}

void UWidgetInteract::OnControl()
{
	AInteractActorBase* Prop = Cast<AInteractActorBase>(m_Focused.GetObject());
	
	Prop->Control();
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

void UWidgetInteract::AutoToggle()
{
	m_AutoToggle=!m_AutoToggle;
	UMyLib::GetPlayer()->SetAutoCombat(m_AutoToggle);
	UMyGameInstance::Get->m_SkillAuto->SetUseAuto(m_AutoToggle);
}