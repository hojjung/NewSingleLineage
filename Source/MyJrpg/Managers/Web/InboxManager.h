// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InboxManager.generated.h"


USTRUCT(BlueprintType)
struct FInboxSpec 
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString MailName;
	UPROPERTY(EditAnywhere)
	FString MailDesc;
	UPROPERTY(EditAnywhere)
	FString ItemID;
	UPROPERTY(EditAnywhere)
	FString ItemAmount;
	UPROPERTY(EditAnywhere)
	FString ExpireTime;
	
	FString m_StrRemainTime;

	bool m_bIsExpired = false;
};
UCLASS()
class MYJRPG_API UInboxManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInboxUpdated,const TArray<FInboxSpec>&);

	UInboxManager();
	
	FOnInboxUpdated m_OnInboxUpdated;
	
protected:
	TArray<FInboxSpec> m_AryInbox;

protected:
	void ClaimItem(FString itemID,int amount);

	void ClaimAllItems();
	
public:
	void SetInboxManager(TArray<FInboxSpec>& inboxSpec);

	void ClaimInbox(int index);
	

	void ClaimAllInbox();

	FORCEINLINE const TArray<FInboxSpec>& GetAryInbox()
	{
		return m_AryInbox;
	}
};
