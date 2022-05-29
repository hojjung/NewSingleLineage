// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetLogWindow.h"

#include "WidgetLogWindow.h"
#include "TimerManager.h"
#include "Kismet/KismetStringLibrary.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "Sound/SoundBase.h"
#include "BUITween.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/ZoneData.h"

void UWidgetLogWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetLogWindow::CloseDialogue);

	m_BtnClose->IsFocusable = false;
}

void UWidgetLogWindow::NativeDestruct()
{
	Super::NativeDestruct();

	if (InDialogue)
	{
		InDialogue->CleanOuter();
	}

	m_SpeakerNode = nullptr;
}

void UWidgetLogWindow::StartDialogue(AActor* speaker, UDialogue* dial)
{
	Super::StartDialogue(speaker, dial);

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UBUITween::Create(this, 0.25f)
		.FromScale(FVector2D(0.1f, 0.1f))
		.ToScale(FVector2D(1.0f, 1.0f))
		.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
		{
			Cast<UWidgetLogWindow>(Owner)->StartDialogueWrap();
		}))
		.Begin();
}

void UWidgetLogWindow::StartDialogueWrap()
{
	InDialogue->AssignPersistentOuter(UMyGameInstance::Get);

	if (InDialogue->m_AryData.Num() <= 1)
	{
		return;
	}
	
	m_AryReplies.Reset();
	
	TArray<const FDialogueNode*> ChildNodes = InDialogue->GetNextNodes(InDialogue->m_AryData[0]);
	
	CreateSpeakerReply(*ChildNodes[0]);//RootNode
}

void UWidgetLogWindow::RenderNextDialogue(const FDialogueNode& playerRply)//재귀적으로 계속해서 불릴것
{
	for(UWidgetLogReply* Rply : m_AryReplies)
	{
		Rply->SetIsEnabled(false);
	}

	TryRunQuestEvent(playerRply);
	
	
	TArray<const FDialogueNode*> AryNodesW = InDialogue->GetNextNodes(playerRply); //다음거

	if(AryNodesW.Num()<=0)
	{
		CloseDialogue();

		return;
	}

	float Delay = 0;
	for (const FDialogueNode* NodeW : AryNodesW)
	{
		if (CheckNodeConditionEnable(*NodeW)) //컨디션이 활성화 안되었아면 선택지가 안나온다.
		{
			UWidgetLogReply* Reply = CreateSpeakerReply(*NodeW);

			UBUITween::Create(Reply, 0.2f, Delay)
			.FromScale(FVector2D(0.f, 0.f))
			.ToScale(FVector2D(1.f, 1.f))
			.Begin();

			Delay += 0.1f;
		}
	}

	m_ScrollReply->ScrollToEnd();
}

void UWidgetLogWindow::ClearReplies()
{
	m_AryReplies.Reset();
	
	m_ScrollReply->ClearChildren();
}

void UWidgetLogWindow::CloseDialogue()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);

	ClearReplies();

	//m_TextDialogue->SetText(FText::FromString(" "));

	NPCActor = nullptr;

	InDialogue = nullptr;

	UBUITween::Create(this, 0.25f)
		.FromScale(FVector2D(1.f, 1.f))
		.ToScale(FVector2D(0.f, 0.f))
		.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
		{
			Cast<UWidgetLogWindow>(Owner)->CloseDialogueWrap();
		}))
		.Begin();
}

void UWidgetLogWindow::CloseDialogueWrap()
{
	SetVisibility(ESlateVisibility::Collapsed);

	m_OnClose.ExecuteIfBound();
}

UWidgetLogReply* UWidgetLogWindow::CreateSpeakerReply(const FDialogueNode& rr)
{
	UWidgetLogReply* LogRplyCreated = CreateWidget<UWidgetLogReply>(this, m_ClassWidgetLogReply);

	LogRplyCreated->Init(rr, this);
	
	LogRplyCreated->m_OnClick.AddUObject(this, &UWidgetLogWindow::RenderNextDialogue);

	m_ScrollReply->AddChild(LogRplyCreated);

	m_AryReplies.Add(LogRplyCreated);

	return LogRplyCreated;
}

void UWidgetLogWindow::TryRunQuestEvent(const FDialogueNode& rply)
{
	if(rply.m_QuestID == NAME_None)
	{
		return;
	}

	//퀘스트 수락창이 떠야한다
	UMyLib::GetCanvas()->TryAcceptQuest(rply.m_QuestID);
}

void UWidgetLogWindow::CompleteTyping()
{
	Super::CompleteTyping();

	if (m_ScrollReply->GetChildrenCount() < 1)
	{
		//m_ImgTypeDone->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetLogWindow::OnClickContinue()
{
	if (m_ScrollReply->GetChildrenCount() > 0)
	{
		return;
	}

	if (GetWorld()->GetTimerManager().GetTimerRemaining(m_TypeTimerHandle) != -1) // 제대로 안되는듯
	{
		CompleteTyping();
	}
	else
	{
		if (m_SpeakerNode)
		{
			RenderNextDialogue(*m_SpeakerNode);
		}
	}
}
