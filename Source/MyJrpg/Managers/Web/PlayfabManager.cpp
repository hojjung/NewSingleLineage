#include "PlayfabManager.h"
#include "JsonObjectConverter.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabClientModels.h"
#include "PlayFabJsonHelpers.h"
#include "PlayFabJsonObject.h"
#include "PlayFabJsonValue.h"
#include "PlayFabServerDataModels.h"
#include "PlayFabUtilities.h"
#include "SessionTicket.h"
#include "Misc/Base64.h"
#include "Core/PlayFabSettings.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
//com.HereticByte.VagabondSurvival
#define LOCTEXT_NAMESPACE "PlayfabManager"

void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	
	if(ErrorResult.ErrorCode == PlayFab::PlayFabErrorCode::PlayFabErrorProfaneDisplayName
		|| ErrorResult.ErrorCode == PlayFab::PlayFabErrorCode::PlayFabErrorInvalidDisplayNameRandomSuffixLength
		|| ErrorResult.ErrorCode == PlayFab::PlayFabErrorCode::PlayFabErrorAllowNonUniquePlayerDisplayNamesDisableNotAllowed)
	{
		m_OnNickNameFail.ExecuteIfBound();
	}
}

UPlayfabManager::UPlayfabManager()
{
	m_CurrentVersionName=TEXT("1");

	m_bIsServerClosed = false;
}

void UPlayfabManager::Init()
{
	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	m_Auth = USessionTicket::CreateAuthCon();
}

void UPlayfabManager::StartPlayfabLogin(FOnLoginEnd dele)
{
	m_OnLoginEnd = dele;
#if PLATFORM_WINDOWS
	UMyLib::PrintInfoText(LOCTEXT("Try Login With Desktop", "로그인 시도-PC"));
	

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = TEXT("JungPC TestID3");
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	request.AuthenticationContext =	m_Auth;
	
	GetClientAPI->LoginWithCustomID(request,FLoginDele::CreateUObject(this, &UPlayfabManager::OnSuccessPlayfabLogin),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

#endif

	
#if PLATFORM_ANDROID
	//SessionTicket 로그인 시도
	UMyLib::PrintInfoText(LOCTEXT("Try Login With Session TIcket","로그인 시도-Google"));

	PRINTF("로그인 시도-Google");
	
	PlayFab::ClientModels::FLoginWithGoogleAccountRequest SessionRequest;
	
	SessionRequest.CreateAccount = false;
	
	SessionRequest.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	SessionRequest.AuthenticationContext = m_Auth;

	GetClientAPI->LoginWithGoogleAccount(SessionRequest,
													PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
														this, &UPlayfabManager::OnSuccessPlayfabLogin),
													PlayFab::FPlayFabErrorDelegate::CreateUObject(
														this, &UPlayfabManager::OnSessionLoginErrorPlayfabReq)
	);
#endif
}

void UPlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error)
{
	if (error.bSucceeded)
	{
		UMyLib::PrintInfoText(LOCTEXT("SUCCESS-GoogleLogin", "구글 로그인 성공"), FLinearColor::Green);
		
		TryLoginPlayfabGoogle();
	}
	else
	{
		UMyLib::PrintInfoText(LOCTEXT("FAIL-GoogleLoginFail-2", "실패-앱을 종료후 구글 계정 로그인 먼저해주세요"));
	}
}

void UPlayfabManager::TryLoginPlayfabGoogle()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();

	PlayFab::ClientModels::FLoginWithGoogleAccountRequest request;
	request.CreateAccount = true;
	request.ServerAuthCode = OnlineIdentity->GetAuthToken(0);
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;

	GetClientAPI->LoginWithGoogleAccount(request,FLoginDele::CreateUObject(this, &UPlayfabManager::OnSuccessPlayfabLogin),
													   FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result)
{
	UMyLib::PrintInfoText(LOCTEXT("SUCCESS-Playfab Login Success", "플레이팹 로그인 성공"), FLinearColor::Green);

	FString SeTicket = Result.SessionTicket;

	m_Auth = USessionTicket::CreateAuthCon(&SeTicket);

	m_PlayfabID = Result.PlayFabId;

	m_LastLoginTime = Result.LastLoginTime;
	
	FTimespan KoreanTime(9,0,0);
	
	m_LastLoginTime+=KoreanTime;
	
	UMyGameInstance::Get->m_PlayfabManager->RequestGetAccountInfo();
}

void UPlayfabManager::OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		return;
	}
	
	UMyLib::PrintInfoText(LOCTEXT("Try Login With Android","로그인 세션 만료,구글로그인 시도"));
	
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		UMyLib::PrintInfoText(LOCTEXT("FAIL-GoogleLoginFail-1","FAIL-GoogleLoginFail-1"));
		return;
	}

	ExternalUi->ShowLoginUI(0, false, false,FOnLoginUIClosedDelegate::CreateUObject(this, &UPlayfabManager::HandleExternalUIClose));
}
//


void UPlayfabManager::RequestSetNickname(FString str)
{
	UMyLib::PrintInfoText(LOCTEXT("Request Nickname", "닉네임 요청 전송"));
	PlayFab::ClientModels::FUpdateUserTitleDisplayNameRequest DisplayReq;
	DisplayReq.DisplayName = str;
	
	GetClientAPI->UpdateUserTitleDisplayName(DisplayReq,FNicknameDele::CreateUObject(this, &UPlayfabManager::OnNickNameSetSuccess),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result)
{
	m_LoadedNickname = result.DisplayName;
	int InsertIndex = m_LoadedNickname.Len() - 4; 
	m_LoadedNickname.InsertAt(InsertIndex, '#');
}

void UPlayfabManager::RequestGetServerTime()
{
	PlayFab::ClientModels::FGetTimeRequest Req;
	 	GetClientAPI->GetTime(Req,
	 		PlayFab::UPlayFabClientAPI::FGetTimeDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessTimeGet),
	 		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt)
{
	m_CurrentTime = rslt.Time;
	
	FTimespan KoreanTime(9,0,0);
	
	m_CurrentTime+=KoreanTime;
}


void UPlayfabManager::RequestVersionCheck()
{
		PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
		
		Req.FunctionName =TEXT( "CheckVersion");
		
		Req.GeneratePlayStreamEvent = true;
		
		GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnVersionCheckCloudScriptSuccess),
			FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	m_ServerVersion = JsonObject->GetStringField(TEXT("ServerVersionName"));
	
	if(m_CurrentVersionName == m_ServerVersion)
	{
		PRINTF("Version Same");
	}
	else
	{
		UMyLib::PrintInfoText(LOCTEXT("Version Changed", "업데이트 해주세요!"), FLinearColor::Yellow);
	}
}


void UPlayfabManager::RequestServerOpenCheck()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName =TEXT( "CheckServerOpen");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnServerCloseCheckScriptSuccess),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	bool IsServerOpen = JsonObject->GetBoolField(TEXT("IsServerOpen"));
	
	m_bIsServerClosed = !IsServerOpen;

	m_ServerCloseOpenTime = JsonObject->GetStringField(TEXT("ServerCloseOpenTime"));

	if(!m_bIsServerClosed)
	{
		UMyLib::PrintInfoText(LOCTEXT("Server Open", "서버 사용 가능") , FLinearColor::Green);
	}
	else
	{
		UMyLib::PrintInfoText(LOCTEXT("Server Closed", "서버 사용 불가"), FLinearColor::Red);
	}
}


void UPlayfabManager::RequestGetAccountInfo()
{
	UMyLib::PrintInfoText(LOCTEXT("RequestGetAccountInfo", "계정 정보 요청"));
	
	FGetAccntInfoReq Req;
	
	GetClientAPI->GetAccountInfo(Req, FGetAccntInfoDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetAccountInfo),
	                             FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

const FString& UPlayfabManager::GetNickName()
{
	return m_LoadedNickname;
}

void UPlayfabManager::OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt)
{
	if (rslt.AccountInfo->TitleInfo->isBanned)
	{
		UMyLib::PrintInfoText(LOCTEXT("Banned Player", "정지된 플레이어"), FLinearColor::Red);
		return;
	}

	if (rslt.AccountInfo->TitleInfo->DisplayName.IsEmpty())
	{
		m_OnLoginEnd.ExecuteIfBound();
		return;
	}

	UMyLib::PrintInfoText(LOCTEXT("Welcome", "환영 합니다"), FLinearColor::Green);

	PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult ResultNickname;
	
	ResultNickname.DisplayName = rslt.AccountInfo->TitleInfo->DisplayName;
	
	OnNickNameSetSuccess(ResultNickname);
}


void UPlayfabManager::RequestTitleNews(FNewsDele onEnd)
{
	PlayFab::ClientModels::FGetTitleNewsRequest Req;
	Req.Count = 5;
	GetClientAPI->GetTitleNews(Req, onEnd, FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

#undef LOCTEXT_NAMESPACE