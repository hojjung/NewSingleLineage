// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "ChatManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UChatManager : public UObject
{
	GENERATED_BODY()
	
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChatReceive, const FString&);
	
	FOnChatReceive m_OnChatReceive;
	
	UChatManager();

protected:
	bool m_bIsWaitingGetChatRequest;

	FString DocsURL;
	
	FString WebURL;
	
	FHttpModule* Http;

	float m_fDeltaCounter;

protected:
	void HttpCall(const FString& URL, FString Type, const FString* formStrData=nullptr);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void FilterBadWord(FString& outChatWant);

public:
	void Tick(float deltaTime);//should split
	
	void RequestGetChatFromServer();
	
	void ChatPost(const FText& chatWant);

	FString URLEncode(FString url);

	FString URLDecode(FString url);

	void SummonChatPost(const FText& gachaName);
};
