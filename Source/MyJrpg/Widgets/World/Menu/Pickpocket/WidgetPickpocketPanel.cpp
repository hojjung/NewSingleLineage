#include "WidgetPickpocketPanel.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPickpocketPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidgetPickpocketPanel::SetTargetPawn(AMonsterPawn* targetPawn)
{
	m_TargetPawn = targetPawn;

	SetTargetInven(m_TargetPawn->GetInven());
	
	UpdateText();
}

AMonsterPawn* UWidgetPickpocketPanel::GetCurrentTargetPawn()
{
	return m_TargetPawn.Get();
}

void UWidgetPickpocketPanel::UpdateText()
{
	FText PocketText = NSLOCTEXT("UWidgetPickpocketPanel","Pocket","의 주머니");
	
	FText OwnerNameText = m_TargetPawn->GetPawnName();
	
	FString StorageStr = FString::Printf(TEXT("%s%s"),*OwnerNameText.ToString(),*PocketText.ToString());
	//
	m_TxtStorageInvenCount->SetText(FText::FromString(StorageStr));
}

void UWidgetPickpocketPanel::OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	if(m_TargetPawn->IsAlive())
	{
		ele->SetTextFocus(NSLOCTEXT("UWidgetPickpocketPanel","Put","넣어주기?"));
	}
	else
	{
		ele->SetTextFocus(NSLOCTEXT("UWidgetPickpocketPanel","Deposite","넣기?"));
	}
}

void UWidgetPickpocketPanel::OnStorageInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	if(m_TargetPawn->IsAlive())
	{
		FText PerText = NSLOCTEXT("UWidgetPickpocketPanel","Steal","훔치기:");
		
		float PercentResult = GetSuccessPercent(UMyLib::GetItemData(inven->GetItemRef(index).m_ID));
		
		FString PerPocket = FString::Printf(TEXT("%s%2.1f"),*PerText.ToString(), PercentResult);
		
		ele->SetTextFocus(FText::FromString(PerPocket));
	}
	else
	{
		ele->SetTextFocus(NSLOCTEXT("UWidgetPickpocketPanel","WithDraw","꺼내기?"));
	}
}

void UWidgetPickpocketPanel::OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	Super::OnPlInvenFocuseConfirm(ele, inven, index);
	//넣어주고 무슨일 생김?
	if(m_TargetPawn->IsAlive())
	{
		PRINTF("UWidgetPickpocketPanel::Put!");
	}
}

void UWidgetPickpocketPanel::OnStorageFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	//레벨마다 등급차이 존재?,레벨 30 일반템 90% 훔치기, 레벨 45 초록템 80% 훔치기, 레벨 60 파랑템 70%훔치기, 레벨 75 빨강템 55%, 레벨 90 보라템 40%, 주활 1%
	if(m_TargetPawn->IsAlive())
	{
		float PercentResult = GetSuccessPercent(UMyLib::GetItemData(inven->GetItemRef(index).m_ID));

		float RandPer = FMath::RandRange(0,100);

		if(RandPer > PercentResult)
		{
			PRINTF("UWidgetPickpocketPanel:: You Failed PickPocket");
			return;//Fail Pickpocket
		}
		
	}
	Super::OnStorageFocuseConfirm(ele, inven, index);
}

float UWidgetPickpocketPanel::GetSuccessPercent(const FItemDataRow& itemData)
{
	const FColorDataRow* ColorData = itemData.m_ColorHandle.GetRow<FColorDataRow>("");

	float PercentData = ColorData->m_fMaxStealPer / ColorData->m_fMaxStealLv;

	float PercentResult = UMyGameInstance::Get->m_PlayerStatManager->GetLevel() * PercentData;

	return PercentResult;
}
