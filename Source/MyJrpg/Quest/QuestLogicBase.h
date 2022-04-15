// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestLogicBase.generated.h"

/*
퀘스트 인스턴스
퀘스트가 등록되면 위젯에 떠야
1. 특정 몬스터 처치
2. 특정 아이템 사용
3. 특정 타입 아이템 제작
4. 장비분해
5. 특정 아이템 착용
6. 특정 타입 장비 강화
7. 상점의 특정 상품 구매
8. 스텟 포인트 사용
9. 특정 레벨 직기
보상을 선택으로 주기
보상 주기를 줄여야한다. 제작 주기도 줄여야함
렙업은 오래 걸리게?
한 티어의 아이템으로 넘어갈때마다 복리 15%씩 오래걸리게?
일반템 10분,고급템 12븐,14분,16분
 14- 28- 56- 112- 224- 448- 896
일반-고급-희귀-영웅-전설-신화-원시
일단 플탐을 더 짧게해서 스스로 플레이 해보기
10분의 1로? 30시간짜리가 3시간?
 */

struct FQuestSpec;
UCLASS(Abstract)
class MYJRPG_API UQuestLogicBase : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnProgressChanged);

	FOnProgressChanged m_OnProgressChanged;
	
	FOnProgressChanged m_OnQuestCompleteable;

protected:
	FQuestSpec* m_Spec = nullptr;

	FDelegateHandle m_QuestDeleHandle;
	
protected:
	virtual void IncreaseAmount();

	virtual void ReceiveRewards();
	
public:
	virtual void SetAmount(int v);
	
	virtual void RegisterQuest(FQuestSpec& spec){};

	virtual bool CanCompleteQuest();

	virtual void CompleteQuest();

	const FQuestSpec& GetQuestSpec() const;

	float GetProgress();

	FText GetProgressText();
};
