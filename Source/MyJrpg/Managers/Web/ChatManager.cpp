#include "ChatManager.h"

#include "Interfaces/IHttpResponse.h"
#include "MyJrpg/Managers/MyGameInstance.h"

UChatManager::UChatManager()
{
	//m_bIsReceiveChatFromServer=false;
	m_bIsWaitingGetChatRequest = false;

	Http = &FHttpModule::Get();

	DocsURL = TEXT("https://docs.google.com/spreadsheets/d/18XUGwRb4DYjyJEpBZL0H3FqvlcDedE8icg7WJ_JNdD0/export?format=tsv&range=B:B");

	WebURL = TEXT("https://script.google.com/macros/s/AKfycbzSr3dVDB0LGxH2yrdgI4sGxEVCbzfkrsHnP854jR4Py-TTadj-Y-OFLW-nBVjSMSWP/exec");

	m_fDeltaCounter = 0.f;
}

void UChatManager::HttpCall(const FString& URL, FString Type, const FString* formStrData)
{
	auto Request = Http->CreateRequest();
	
	Request->OnProcessRequestComplete().BindUObject(this, &UChatManager::OnResponseReceived);

	Request->SetURL(URL);
	Request->SetVerb(Type);

	if (formStrData)
	{
		FString NewStr = *formStrData;

		NewStr = URLEncode(NewStr);

		Request->SetContentAsString(NewStr);
	}

	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
	
	Request->ProcessRequest();
}

void UChatManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Request->GetVerb()=="GET") //for split from chat post
	{
		if(!m_bIsWaitingGetChatRequest)
		{
			return;
		}
		m_bIsWaitingGetChatRequest = false;

		if (bWasSuccessful)
		{
			FString ChatContent = Response->GetContentAsString();

			m_OnChatReceive.Broadcast(ChatContent);
		}
	}
	else if (Request->GetVerb()=="POST")
	{
		
	}
}

void UChatManager::RequestGetChatFromServer()
{
	HttpCall(DocsURL, "GET");
	m_bIsWaitingGetChatRequest = true;
	m_fDeltaCounter = 0.f;
}

void UChatManager::FilterBadWord(FString& outChatWant)
{
	UMyGameInstance::Get->m_BadwordTable->ChatFilterBadWord(outChatWant);
}

void UChatManager::Tick(float deltaTime)
{
	m_fDeltaCounter += deltaTime;

	if (m_fDeltaCounter > 2.0f)
	{
		if (m_bIsWaitingGetChatRequest)
		{
			return;
		}
		RequestGetChatFromServer();
	}
}

FString UChatManager::URLEncode(FString url)
{
	FString dst;
	char hex[] = "0123456789ABCDEF";
	TArray<ANSICHAR> ANSIArray;

	int32 SrcLen = TCString<ANSICHAR>::Strlen(TCHAR_TO_UTF8(*url)) + 1;
	ANSIArray.AddUninitialized(SrcLen);

	FPlatformString::Convert(ANSIArray.GetData(), SrcLen, TCHAR_TO_UTF8(*url), SrcLen);

	int32 index = 0;

	for (size_t i = 0; i < ANSIArray.Num(); ++i)
	{
		if (isascii(ANSIArray[i])
			&& ANSIArray[i] != '[' && ANSIArray[i] != ']'
			&& ANSIArray[i] != '{' && ANSIArray[i] != '}'
			&& ANSIArray[i] != '\"')
		{
			dst.AppendChar(ANSIArray[i]);
		}
		else
		{
			uint8 c(ANSIArray[i]);
			dst.AppendChar('%');
			dst.AppendChar(hex[c / 16]);
			dst.AppendChar(hex[c % 16]);
		}
	}
	return (dst);
}

FString UChatManager::URLDecode(FString url)
{
	TArray<ANSICHAR> returnStr1;
	
	for (int32 index = 0; index < url.Len(); index++)
	{
		FString tempStr("");
		if (url[index] != '%')
		{
			returnStr1.Add(url[index]);
			//s[index2] = url[index];
		}
		else if (isxdigit(url[index + 1]) && isxdigit(url[index + 2]))
		{
			tempStr += url[index + 1];
			tempStr += url[index + 2];
			int32 hex = FCString::Strtoi(*tempStr, 0, 16);


			returnStr1.Add(ANSICHAR(hex));
			//s[index2] = char(hex);

			index += 2;
		}
		else
		{
			returnStr1.Add('%');
			//s[index2] = '%';
		}
		//index2++;
	}
	returnStr1.Add('\0');
	FString str = UTF8_TO_TCHAR(returnStr1.GetData());
	//delete[]s;
	return str;
}

void UChatManager::ChatPost(const FText& chatWant)
{
	FText CachedText = chatWant;

	FString CachedString = CachedText.ToString();

	FilterBadWord(CachedString);

	FString FormatStr = FString::Printf(TEXT("ranking=%d&nickname=%s&chat=%s"), 1,TEXT("TestName"), *CachedString);

	HttpCall(WebURL, "POST", &FormatStr);

	m_fDeltaCounter=0.f;
	
	m_bIsWaitingGetChatRequest = false;
}

void UChatManager::SummonChatPost(const FText& gachaName)
{
	FText CachedText = gachaName;

	FString CachedString = CachedText.ToString();

	FilterBadWord(CachedString);

	FString FormatStr;// = FString::Printf(TEXT("nickname=%s&gacha=%s"), *UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname, *CachedString);

	HttpCall(WebURL, "POST", &FormatStr);

	m_fDeltaCounter=0.f;
	
	m_bIsWaitingGetChatRequest = false;
}

