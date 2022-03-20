#include "WidgetChatElement.h"

void UWidgetChatElement::SetChat(const FString& chat)
{
	m_TextChat->SetDefaultColorAndOpacity(FLinearColor::White);
	FString CachedChat = chat;
	//CachedChat = CachedChat.TrimStart();
	CachedChat.RemoveFromEnd(TEXT("\r"));
	
	TArray<FString> OutStrAry;
	
	CachedChat.ParseIntoArray(OutStrAry,TEXT(":"));

	if(OutStrAry.Num()>2)
	{
		FString Ranking = OutStrAry[0];
		
		int Rank = FCString::Atoi(*Ranking);

		FString Nickname = OutStrAry[1];

		FString ChatText = OutStrAry[2];

		FString FormatStr;

		if(Rank>9999)
		{
			FormatStr = FString::Printf(TEXT("[초짜]:%s:%s"),*Nickname,*ChatText);
		}
		else
		{
			FormatStr = FString::Printf(TEXT("<%s>[%sst]</>:%s:%s"),*GetRankColor(Ranking),*Ranking,*Nickname,*ChatText);	
		}
		m_TextChat->SetText(FText::FromString(FormatStr));
	}
	else
	{
		m_TextChat->SetText(FText::FromString(CachedChat));
		
		m_TextChat->SetDefaultColorAndOpacity(FLinearColor::Yellow);
	}
}

void UWidgetChatElement::Clear()
{
	m_TextChat->SetText(FText());
}

FString UWidgetChatElement::GetRankColor(const FString& rankNumber)
{
	int RankNumber = FCString::Atoi(*rankNumber);

	return GetRankColor(RankNumber);
}

FString UWidgetChatElement::GetRankColor(int RankNumber)
{
	if(RankNumber==1)
	{
		return TEXT("Purple");
	}
	else if(RankNumber>=2 && RankNumber<10)
	{
		return TEXT("Red");;
	}
	else if(RankNumber>=10&&RankNumber<100)
	{
		return TEXT("Orange");
	}
	else if(RankNumber>=100&&RankNumber<1000)
	{
		return TEXT("Yellow");
	}
	else if(RankNumber>=1000&&RankNumber<5000)
	{
		return TEXT("Blue");
	}
	else if(RankNumber>=5000&&RankNumber<10000)
	{
		return TEXT("Green");
	}

	return TEXT("Default");
}
