#include "AdmobManager.h"
//
// #define ADSCOOL 720.f
//
// FString UAdmobManager::GetTimeToStr()
// {
// 	int Minutes = m_fRemainAdmobTime / 60.f;
//
// 	int Secs = m_fRemainAdmobTime - (Minutes*60.f);
// 	
// 	return FString::Printf(TEXT("쿨다운:%.2i:%.2i"),Minutes,Secs);
// }
//
// void UAdmobManager::Init()
// {
// 	m_RewardAdID = TEXT("ca-app-pub-2129135695247377/1075261375");
// 	//
// 	m_bLoadRewardAdsProcessing=false;
//
// 	m_bAdsUpdateLock=false;
// }
//
// void UAdmobManager::SetTimeCooldownFromServer(FDateTime lastAdmobTime, FDateTime currentLoginTime)
// {
// 	m_fRemainAdmobTime = 0.f;
// 	//로그인후 클레임시,양수,이후 로그아웃 로그인하면 음수가 되버림
// 	FTimespan AdmobRemainTime =  lastAdmobTime-currentLoginTime;//이시간은 얼마든지 달라질수있음
//
// 	m_fRemainAdmobTime=ADSCOOL-AdmobRemainTime.GetDuration().GetTotalSeconds();
//
// 	m_fRemainAdmobTime = FMath::Clamp(m_fRemainAdmobTime,0.f,ADSCOOL);
// }
//
// void UAdmobManager::Tick(float deltaTime)
// {
// 	m_fRemainAdmobTime-=deltaTime;
//
// 	if(m_fRemainAdmobTime>0)
// 	{
// 		m_OnAdTick.Broadcast(GetTimeToStr());
// 	}
// 	else if(!m_bAdsUpdateLock)
// 	{
// 		m_bAdsUpdateLock=true;
//
// 		m_OnAdTick.Broadcast(FString());
// 	}
// }
//
// void UAdmobManager::ShowBannerAD(bool b)
// {
// 	// bool IsPurchased = UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(0);
// 	// //
// 	// if (b && (!GetDefault<UPlayFabRuntimeSettings>()->bIsVIPGameVersion) && !IsPurchased)
// 	// {
// 	// 	UEasyAdsLibrary::ShowBanner(false);
// 	// 	m_OnShowAdBanner.Broadcast(true);
// 	// }
// 	// else
// 	// {
// 	// 	UEasyAdsLibrary::HideBanner();
// 	//
// 	// 	m_OnShowAdBanner.Broadcast(false);
// 	// }
// }
//
// void UAdmobManager::ShowRewardAds()
// {
// 	
// 	if(m_fRemainAdmobTime>0)
// 	{
// 		return;
// 	}
// 	
// 	if(m_bLoadRewardAdsProcessing)
// 	{
// 	 	return;
// 	}
//
// 	//UDiabloGameInstance::Get->GetHud()->ShowTouchBan(1.2f);
// 	
// 	m_bLoadRewardAdsProcessing=true;
// 	
// 	m_bAdsUpdateLock=false;
// 	
// 	m_LoadedRewardAds=ULoadCustomRewardedVideoAd::LoadCustomRewardedVideoAd(m_RewardAdID);
// 	m_LoadedRewardAds->OnLoadSuccess.AddDynamic(this,&UAdmobManager::RewardAdsLoadSuccess);
// 	m_LoadedRewardAds->OnLoadFail.AddDynamic(this,&UAdmobManager::RewardAdsLoadFail);
// 	m_LoadedRewardAds->Activate();
// }
//
// void UAdmobManager::RewardAdsLoadSuccess()
// {
// 	UPlayCustomRewardedVideoAd* PlayVideoProxy = UPlayCustomRewardedVideoAd::PlayCustomRewardedVideoAd(m_RewardAdID);
// 	PlayVideoProxy->OnComplete.AddDynamic(this,&UAdmobManager::OnRewardAdsSuccess);
// 	PlayVideoProxy->OnFail.AddDynamic(this,&UAdmobManager::OnRewardAdsPlayFail);
// 	PlayVideoProxy->OnClose.AddDynamic(this,&UAdmobManager::OnRewardAdsClose);
// 	PlayVideoProxy->Activate();
// }
//
// void UAdmobManager::RewardAdsLoadFail(FString reason)
// {
// 	//UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 로딩 실패,그냥 받으세요"));
// 	
// 	m_bLoadRewardAdsProcessing=false;
//
// 	OnRewardAdsSuccess(FString(),0);
// }
//
// void UAdmobManager::OnRewardAdsSuccess(FString item, int32 amount)
// {
// 	m_bLoadRewardAdsProcessing=false;
// 	
// 	int RandomGemStones = FMath::RandRange(30,150);
// 	//UDiabloGameInstance::Get->RequestPopupText(FString::Printf(TEXT("광고 시청성공,젬스톤%d개 획득"),RandomGemStones));
// 	
// 	//UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(RandomGemStones);
// 	//
// 	//UDiabloGameInstance::Get->m_PlayfabManager->UploadAdmobTime(FDateTime::UtcNow());
// 	
// 	m_fRemainAdmobTime=ADSCOOL;
// 	
// 	m_bAdsUpdateLock=false;
// }
//
// void UAdmobManager::OnRewardAdsClose()
// {
// 	//UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 창 닫힘"));
// 	m_bLoadRewardAdsProcessing=false;
// }
//
// void UAdmobManager::OnRewardAdsPlayFail(FString reason)
// {
// 	//UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 재생 실패"));
// 	m_bLoadRewardAdsProcessing=false;
// }
//
