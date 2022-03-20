#include "WidgetChatWindow.h"

#include "Components/ScrollBoxSlot.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetChatWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_SendButton->OnClicked.AddDynamic(this,&UWidgetChatWindow::SendText);
	
	UMyGameInstance::Get->m_ChatManager->m_OnChatReceive.AddUObject(this,&UWidgetChatWindow::OnReceiveTotalChatList);
	//
	for(int i=0; i< 10; i++)
	{
		UWidgetChatElement* CreatedChat = CreateWidget<UWidgetChatElement>(this, m_ClassTextWidget);

		UScrollBoxSlot* SlotUni = Cast<UScrollBoxSlot>( m_MessageScrollBox->AddChild(CreatedChat));//20//15

		SlotUni->SetPadding(FMargin(0,15.f,0,0));

		m_AryChatText.Add(CreatedChat);
	}
	m_nTopIndex=0;

	m_MesageEditableText->OnTextChanged.AddDynamic(this,&UWidgetChatWindow::OnChatTextChanged);

	m_bIsOpened=true;

	SetChatWindowOpenClose();

	m_ChatWindowToggle->OnClicked.AddDynamic(this,&UWidgetChatWindow::SetChatWindowOpenClose);
}

void UWidgetChatWindow::ClearChat()
{
	for(UWidgetChatElement* TextChat : m_AryChatText)
	{
		TextChat->Clear();
	}
}

UWidgetChatElement* UWidgetChatWindow::GetTopText()
{
	UWidgetChatElement* Text = m_AryChatText[m_nTopIndex];

	m_nTopIndex++;

	if(m_nTopIndex>=m_AryChatText.Num())
	{
		m_nTopIndex=0;
	}
	return Text;
}

void UWidgetChatWindow::OnReceiveTotalChatList(const FString& chat)
{
	ClearChat();
	
	TArray<FString> OutStrAry;
	
	chat.ParseIntoArray(OutStrAry,TEXT("\n"));

	for(FString& Str : OutStrAry)
	{
		AddTextWidget(Str);
	}
}
void UWidgetChatWindow::SendText()
{
	FText ChatText = m_MesageEditableText->GetText();

	if(ChatText.IsEmpty())
	{
		return;
	}

	AddTextWidget(ChatText.ToString());
	
	UMyGameInstance::Get->m_ChatManager->ChatPost(ChatText);

	m_MesageEditableText->SetText(FText());

	m_MessageScrollBox->ScrollToEnd();
}

void UWidgetChatWindow::AddTextWidget(const FString& chat)
{
	UWidgetChatElement* TopText = GetTopText();
	
	TopText->SetChat(chat);

	m_MessageScrollBox->AddChild(TopText);
}

void UWidgetChatWindow::OnChatTextChanged(const FText& text)
{
	FString Str = text.ToString();
	
	int Diff = Str.Len() - 40;
	
	if(Diff>0)
	{
		Str = Str.LeftChop(Diff);
	}

	m_MesageEditableText->SetText(FText::FromString(Str));
}

void UWidgetChatWindow::SetChatWindowOpenClose()
{
	if(m_bIsOpened)
	{
		m_ChatTotalWindow->SetVisibility(ESlateVisibility::Collapsed);	
	}
	else
	{
		m_ChatTotalWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	m_bIsOpened =!m_bIsOpened;
}