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

	DECLARE_DELEGATE_OneParam(FOnPlayerLogin, bool);

	FOnPlayerLogin m_PlayerLogined;

protected:
	PlayFabClientPtr GetClientAPI = nullptr;

	FString m_SessionTicket;

	TArray<PlayFab::ClientModels::FTitleNewsItem> m_TitleNews;

	FString m_LoadedNickname;

	bool m_bIsNicknameSet;

	TSharedPtr<UPlayFabAuthenticationContext> m_Auth;

	bool m_bLoginProcessStarted;

	bool m_bIsNewCreatePlayer;

	FString m_PlayfabID;

	FDateTime m_LastLoginTime;
	
	FDateTime m_LastLogoutTime;
	
 	FDateTime m_CurrentTime;

	bool m_bIsServerClosed;

	FString m_ServerCloseOpenTime;

	FString m_ServerVersion;

	FString m_CurrentVersionName;
	
protected:
	void RequestSetNickname(FString str);

	void OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result);

	void OnStageCompleteScriptSuccess(const FExeCScriptRslt& rslt);

	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void RequestVersionCheck();
	
	void OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt);

	void RequestServerOpenCheck();

	void RequestTitleNews();
	
	void OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt);

	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);
	
	void RequestGetServerTime();

	void RequestGetAccountInfo();

	FDateTime DecodePlayfabTimeToUe4Time(FString playfabTime);
	
	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt);
	
	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);

	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);

	void OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult);

	void StartPlayfabLogin();
	
	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	
 	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

public:
	void Init();
};

