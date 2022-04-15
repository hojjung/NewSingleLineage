// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/QuestData.h"
#include "UObject/NoExportTypes.h"
#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnQuestChanged);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuestAddRemove,UQuestLogicBase*,bool);

	FOnQuestChanged m_OnQuestChanged;

	FOnQuestAddRemove m_OnQuestAdd;

	FOnQuestAddRemove m_OnQuestRemove;

protected:
	TMap<const FQuestDataRow*,FQuestSpec> m_MapMainQuestSpec;
	
	TMap<const FQuestSpec*,UQuestLogicBase*> m_MapMainQuestLogic;
	UPROPERTY()
	TArray<UQuestLogicBase*> m_AryMainQuestLogic;

protected:
	TMap<const FQuestDataRow*,FQuestSpec> m_MapSubQuestSpec;
	
	TMap<const FQuestSpec*,UQuestLogicBase*> m_MapSubQuestLogic;
	UPROPERTY()
	TArray<UQuestLogicBase*> m_ArySubQuestLogic;

	int m_nCurrentMainQuestIndex;
	
protected:
	FName GetMainQuestID(int index);

	void RegisterNextMainquest();
	
	void CompleteMainQuest(UQuestLogicBase* logic);

	void CompleteSubQuest(UQuestLogicBase* logic);
public:
	void Init();

	const FQuestDataRow& GetMainQuestData(FName id) const;

	const FQuestDataRow& GetSubQuestData(FName id) const;
	
	void RegisterMainQuest(FName id);

	void RegisterMainQuestByIndex(int index);

	void RegisterSubQuest(FName id);

	

	void CompleteQuest(UQuestLogicBase* quest);

	void SetMainQuestValue(int v);

	void SetSubQuestValue(FName id,int v);

	UQuestLogicBase* GetCurrentMainQuest();

	FORCEINLINE const TArray<UQuestLogicBase*>& GetMainQuests() const
	{
		return m_AryMainQuestLogic;
	}

	FORCEINLINE const TArray<UQuestLogicBase*>& GetSubQuests() const
	{
		return m_ArySubQuestLogic;
	}
};


