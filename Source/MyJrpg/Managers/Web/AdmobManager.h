// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "EasyAds/Public/EasyAdsLibrary.h"
//#include "EasyAds/Public/LoadCustomInterstitialAd.h"
//#include "EasyAds/Public/LoadCustomRewardedVideoAd.h"
//#include "EasyAds/Public/PlayCustomRewardedVideoAd.h"
//#include "EasyAds/Public/PlayVideoProxy.h"
//#include "EasyAds/Public/ShowCustomInterstitialAd.h"
//#include "EasyAds/Public/ShowInterstitialProxy.h"
#include "AdmobManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAdTick,const FString&);

UCLASS()
class MYJRPG_API UAdmobManager : public UObject
{
	GENERATED_BODY()
// 	
// public:
// 	FOnAdTick m_OnAdTick;
// 	
// protected:
// 	UPROPERTY()
// 	ULoadCustomRewardedVideoAd* m_LoadedRewardAds;
//
// 	bool m_bLoadRewardAdsProcessing;
//
// 	FString m_RewardAdID;
//
// 	float m_fRemainAdmobTime;
//
// 	bool m_bAdsUpdateLock;
//
// public:
// 	DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowAdBanner,bool);
// 	FOnShowAdBanner m_OnShowAdBanner;
// 	
// protected:
// 	FString GetTimeToStr();
// public:
// 	void Init();
//
// 	void SetTimeCooldownFromServer(FDateTime lastAdmobTime,FDateTime currentLoginTime);
//
// 	void Tick(float deltaTime);
// 	
// 	UFUNCTION()
//     void RewardAdsLoadFail(FString reason);
// 	UFUNCTION()
//     void RewardAdsLoadSuccess();
// 	UFUNCTION()
// 	void OnRewardAdsPlayFail(FString reason);
//
// 	void ShowBannerAD(bool b);
//
// 	void ShowRewardAds();
//
// public:
// 	UFUNCTION()
// 	void OnRewardAdsSuccess(FString item, int32 amount);
// 	UFUNCTION()
// 	void OnRewardAdsClose();

	
};
