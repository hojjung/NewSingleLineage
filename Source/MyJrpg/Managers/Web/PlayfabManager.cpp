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
//com.HereticByte.VagabondSurvival
#define LOCTEXT_NAMESPACE "PlayfabManager"

UPlayfabManager::UPlayfabManager()
{
	m_CurrentVersionName=TEXT("1");
}

void UPlayfabManager::Init()
{
	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		UMyLib::PrintInfoText(LOCTEXT("FAIL-GoogleLoginFail-1","FAIL-GoogleLoginFail-1"));
		return;
	}

	ExternalUi->ShowLoginUI(0, false, false,FOnLoginUIClosedDelegate::CreateUObject(this, &UPlayfabManager::HandleExternalUIClose));
}

void UPlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error)
{
	PRINTF("ID:%s",*uniqueId->ToString());	
	if (error.bSucceeded)
	{
		UMyLib::PrintInfoText(LOCTEXT("SUCCESS-GoogleLogin", "구글 로그인 성공01"));
		
		TryLoginPlayfabGoogle(uniqueId);
	}
	else
	{
		UMyLib::PrintInfoText(LOCTEXT("FAIL-GoogleLoginFail-2", "실패-앱을 종료후 구글 계정 로그인 먼저해주세요"));
	}
}

TSharedPtr<UPlayFabAuthenticationContext> UPlayfabManager::CreateAuthCon(const FString* newSessonTicket)
{
	if(newSessonTicket)//로그인후 새로 만드는거임
	{
		PlayFab::PlayFabSettings::SetClientSessionTicket(*newSessonTicket);
		m_SessionTicket = PlayFab::PlayFabSettings::GetClientSessionTicket();
		
		USessionTicket* SaveGameInstance = Cast<USessionTicket>(UGameplayStatics::CreateSaveGameObject(USessionTicket::StaticClass()));
		
		SaveGameInstance->m_SessionTicket = m_SessionTicket;
		
		UGameplayStatics::SaveGameToSlot(SaveGameInstance,TEXT("SessionTicket"),0);
		
		return TSharedPtr<UPlayFabAuthenticationContext>(NewObject<UPlayFabAuthenticationContext>(),DeleterNot());
	}
	else//로그인 하기전 로컬 로드
	{
		USessionTicket* LoadedSession = Cast<USessionTicket>(UGameplayStatics::LoadGameFromSlot(TEXT("SessionTicket"),0));

		if(LoadedSession && !LoadedSession->m_SessionTicket.IsEmpty())//로드 성공
		{
			PlayFab::PlayFabSettings::SetClientSessionTicket(LoadedSession->m_SessionTicket);
			m_SessionTicket = PlayFab::PlayFabSettings::GetClientSessionTicket();
			
			return TSharedPtr<UPlayFabAuthenticationContext>(NewObject<UPlayFabAuthenticationContext>(),DeleterNot());
		}
	}
	
	PRINTF("No Authcontext");

	return nullptr;
}

void UPlayfabManager::RequestSetNickname(FString str)
{
	UMyLib::PrintInfoText(LOCTEXT("Request Nickname", "닉네임 요청 전송"));
	PlayFab::ClientModels::FUpdateUserTitleDisplayNameRequest DisplayReq;

	DisplayReq.DisplayName = str;

	GetClientAPI->UpdateUserTitleDisplayName(DisplayReq,
	                                         PlayFab::UPlayFabClientAPI::FUpdateUserTitleDisplayNameDelegate::CreateUObject(
		                                         this, &UPlayfabManager::OnNickNameSetSuccess)
	                                         , PlayFab::FPlayFabErrorDelegate::CreateUObject(
		                                         this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	PRINTF("99:%s",*CodeString);

	//UMyLib::PrintInfoText(NSLOCTEXT("ATreeBase","No Tool","적합한 도구가 없음"));
	UMyLib::PrintInfoText(CodeString);
	
	if(1074 ==ErrorResult.ErrorCode ||CodeString.IsEmpty())
	{
		StartPlayfabLogin();
		return;	
	}
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
	RequestGetAccountInfo();
}

void UPlayfabManager::OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result)
{
	//result.DisplayName
	UMyLib::PrintInfoText(LOCTEXT("Nickname Success", "닉네임 성공,기다려주세요"));
	m_bIsNicknameSet = true;
	m_LoadedNickname = result.DisplayName;
}

void UPlayfabManager::OnStageCompleteScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}
	FString Result = JsonObject->GetStringField(TEXT("Result"));
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
		RequestTitleNews();
	}
	else
	{
		UMyLib::PrintInfoText(LOCTEXT("Version Changed", "업데이트 해주세요!"));
	}
}

void UPlayfabManager::RequestVersionCheck()
{
		PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
		
		Req.FunctionName =TEXT( "CheckVersion");
		
		Req.GeneratePlayStreamEvent = true;
		
		GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnVersionCheckCloudScriptSuccess),
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
		UMyLib::PrintInfoText(LOCTEXT("Server Open", "서버 사용 가능"));
		
		RequestVersionCheck();
	}
	else
	{
		UMyLib::PrintInfoText(LOCTEXT("Server Closed", "서버 사용 불가"));
	}
}

void UPlayfabManager::OnNewPlayerDataInitSuccess(const FExeCScriptRslt& rslt)
{
	RequestServerOpenCheck();
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

void UPlayfabManager::StartPlayfabLogin()
{
	
#if PLATFORM_WINDOWS
	UMyLib::PrintInfoText(LOCTEXT("Try Login With Desktop", "로그인 시도-PC"));
	

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = TEXT("JungPC TestID3");
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	request.AuthenticationContext =	m_Auth;
	
	bool Result = GetClientAPI->LoginWithCustomID(request,
												PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
													this, &UPlayfabManager::OnSuccessPlayfabLogin),
												PlayFab::FPlayFabErrorDelegate::CreateUObject(
													this, &UPlayfabManager::OnErrorPlayfabReq)
	);


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
//
void UPlayfabManager::TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId) //클라아이디를 키서명으로 바꿔봄
{
	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();

	auto Status = OnlineIdentity->GetLoginStatus(0);

	switch (Status)
	{
	case ELoginStatus::NotLoggedIn:
		{
			PRINTF("33");
			UMyLib::PrintInfoText(LOCTEXT("FAIL-LoginStatus:NotLoggedin",
			                              "FAIL-LoginStatus:NotLoggedin"));
			break;
		}
	case ELoginStatus::UsingLocalProfile:
		{
			PRINTF("44");
			UMyLib::PrintInfoText(LOCTEXT("FAIL-LoginStatus:UsingLocalProfile",
			                              "FAIL-LoginStatus:UsingLocalProfile"));
			break;
			
		}
	case ELoginStatus::LoggedIn:
		{
			PRINTF("55");
			UMyLib::PrintInfoText(LOCTEXT("LoginStatus:LoggedIn", "LoginStatus:LoggedIn"));
			break;
		}
	default:
		{
			PRINTF("66");;
		}
	}
	PlayFab::ClientModels::FLoginWithGoogleAccountRequest request;
	request.CreateAccount = true;
	request.ServerAuthCode = OnlineIdentity->GetAuthToken(0);

	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;

	bool Result = GetClientAPI->LoginWithGoogleAccount(request,
	                                                   PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
		                                                   this, &UPlayfabManager::OnSuccessPlayfabLogin),
	                                                   PlayFab::FPlayFabErrorDelegate::CreateUObject(
		                                                   this, &UPlayfabManager::OnErrorPlayfabReq)
	);


	if (!Result)
	{
		PRINTF("88");;
		UMyLib::PrintInfoText(
			LOCTEXT("Fail-Request PlayfabLogin", "Fail-Request PlayfabLogin"));
	}

}

void UPlayfabManager::OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result)
{
	PRINTF("77");
	UMyLib::PrintInfoText(LOCTEXT("SUCCESS-Playfab Login Success", "구글 로그인 성공02"));

	FString SeTicket = Result.SessionTicket;

	PRINTF("77$$1");
	
	m_Auth = CreateAuthCon(&SeTicket);

	PRINTF("77$$2");
	
	m_PlayfabID = Result.PlayFabId;

	m_LastLoginTime = Result.LastLoginTime;
	
	FTimespan KoreanTime(9,0,0);
	
	m_LastLoginTime+=KoreanTime;
	
	m_bIsNewCreatePlayer = Result.NewlyCreated;

	if(m_bIsNewCreatePlayer)//닉네임 설정떠야함
	{
		
	}
	PRINTF("88");
	RequestGetServerTime();
}
FDateTime UPlayfabManager::DecodePlayfabTimeToUe4Time(FString playfabTime)
{
	FDateTime DateTimeOut;
	FString DateString = playfabTime;
	
	if (!FDateTime::ParseIso8601(*DateString, DateTimeOut))
	{
		PRINTF("TimeChangeFail");
	}

	return DateTimeOut;
}
void UPlayfabManager::RequestGetAccountInfo()
{
	UMyLib::PrintInfoText(LOCTEXT("RequestGetAccountInfo", "계정 정보 요청"));
	FGetAccntInfoReq Req;

	GetClientAPI->GetAccountInfo(Req, FGetAccntInfoDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetAccountInfo),
	                             FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt)
{
	if (rslt.AccountInfo->TitleInfo->isBanned)
	{
		UMyLib::PrintInfoText(LOCTEXT("Banned Player", "정지된 플레이어"));
		return;
	}

	if (rslt.AccountInfo->TitleInfo->DisplayName.IsEmpty())
	{
		UMyLib::PrintInfoText(LOCTEXT("Please Set Nickname", "닉네임을 설정해주세요"));
		return;
	}

	UMyLib::PrintInfoText(LOCTEXT("Welcome", "환영 합니다"));

	m_LoadedNickname = rslt.AccountInfo->TitleInfo->DisplayName;
	
	m_bIsNicknameSet = true;
	
	RequestServerOpenCheck();
}

void UPlayfabManager::RequestTitleNews()
{
	PlayFab::ClientModels::FGetTitleNewsRequest Req;
	Req.Count = 5;
	GetClientAPI->GetTitleNews(Req,PlayFab::UPlayFabClientAPI::FGetTitleNewsDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetTitleNews),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt)
{
	 m_TitleNews=rslt.News;
}

void UPlayfabManager::RequestServerOpenCheck()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName =TEXT( "CheckServerOpen");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnServerCloseCheckScriptSuccess),
        FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

#undef LOCTEXT_NAMESPACE