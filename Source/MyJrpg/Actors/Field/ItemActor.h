// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Widgets/World/Menu/QuickSlotsPanel/WidgetQuickSlot.h"
#include "ItemActor.generated.h"

UCLASS()
class MYJRPG_API AItemActor : public AActor, public IFocusable
{
	GENERATED_BODY()
	
public:	
	AItemActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBillboardComponent* m_BillboardIcon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetSpeechBubbleComp* m_SpeechBubbleComp;
	
	FName m_OwnerID;

	FName m_ItemID;

	int m_nCountLevel;

public:
	void Init(FName itemID, int countOrLevel);//그럼 어떻게 스택 아이템 처리?
	
	void Obtain();

	bool HasOwnerTeamID() const;
};

