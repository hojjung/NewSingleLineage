#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineError.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"

#include "Net/UnrealNetwork.h"
#include "Interfaces/OnlineStoreInterface.h"
#include "PlayfabManager.generated.h"


typedef PlayFab::UPlayFabClientAPI::FGetTitleNewsDelegate FNewsDele;

typedef PlayFab::UPlayFabClientAPI::FUpdateUserTitleDisplayNameDelegate FNicknameDele;

typedef PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate FLoginDele;


typedef PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate FUpdateDele;
typedef PlayFab::ClientModels::FUpdateUserDataRequest FUpdateReq;
typedef PlayFab::ClientModels::FUpdateUserDataResult FUpdateRslt;
//
typedef PlayFab::UPlayFabClientAPI::FExecuteCloudScriptDelegate FExeCScriptDele;
typedef PlayFab::ClientModels::FExecuteCloudScriptRequest FExeCScriptReq;
typedef PlayFab::ClientModels::FExecuteCloudScriptResult FExeCScriptRslt;
//
typedef PlayFab::UPlayFabClientAPI::FGetUserDataDelegate FGetUsrDataDele;
typedef PlayFab::ClientModels::FGetUserDataRequest FGetUsrDataReq;
typedef PlayFab::ClientModels::FGetUserDataResult FGetUsrDataRslt;
//
typedef PlayFab::FPlayFabErrorDelegate FFailDele;
typedef PlayFab::FPlayFabCppError FFailRslt;
//
typedef PlayFab::UPlayFabClientAPI::FGetAccountInfoDelegate FGetAccntInfoDele;
typedef PlayFab::ClientModels::FGetAccountInfoRequest FGetAccntInfoReq;
typedef PlayFab::ClientModels::FGetAccountInfoResult FGetAccntInfoRslt;
//

UCLASS()
class MYJRPG_API UPlayfabManager : public UObject
{
	GENERATED_BODY()

public:
	UPlayfabManager();

	DECLARE_DELEGATE(FOnLoginEnd);

	FOnLoginEnd m_OnNickNameFail;
protected:
	PlayFabClientPtr GetClientAPI = nullptr;
	UPROPERTY()
	FString m_SessionTicket;
	UPROPERTY()
	FString m_LoadedNickname;

	TSharedPtr<UPlayFabAuthenticationContext> m_Auth;
	UPROPERTY()
	FString m_PlayfabID;
	UPROPERTY()
	FDateTime m_LastLoginTime;
	UPROPERTY()
	FDateTime m_LastLogoutTime;
	UPROPERTY()
 	FDateTime m_CurrentTime;
	UPROPERTY()
	bool m_bIsServerClosed;
	UPROPERTY()
	FString m_ServerCloseOpenTime;
	UPROPERTY()
	FString m_ServerVersion;
	UPROPERTY()
	FString m_CurrentVersionName;

protected:
	FOnLoginEnd m_OnLoginEnd;

protected:
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);
	
public:
	void RequestSetNickname(FString str);

protected:
	void OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result);

public:
	void RequestTitleNews(FNewsDele onEnd);

protected:
	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void RequestVersionCheck();
	
	void OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt);

	void RequestServerOpenCheck();

	void RequestGetServerTime();


	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt);
	
	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);

	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);

	void OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult);

	
	void TryLoginPlayfabGoogle();
	
 	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

public:
	void Init();
	
	void StartPlayfabLogin(FOnLoginEnd dele);
	
	void RequestGetAccountInfo();

	const FString& GetNickName();
};

