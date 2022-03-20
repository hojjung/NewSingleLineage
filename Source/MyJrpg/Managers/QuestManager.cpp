#include "QuestManager.h"
#include "MyJrpg/DataTables/QuestData.h"




void UQuestManager::Init()
{
	//메인퀘는 전부 저장되서 보여준다?,어떤 퀘스트를깼고 어떤 보상을 받았는지 보여줌
	//서브퀘는 매번 새로 받기
	m_MapMainQuestSpec.Reset();
	m_MapMainQuestLogic.Reset();
	m_AryMainQuestLogic.Reset();
	m_MapSubQuestSpec.Reset();
	m_MapSubQuestLogic.Reset();
	m_ArySubQuestLogic.Reset();
	//
	m_nCurrentMainQuestIndex = 0;
	//세이브 데이터받아옴
	int MainQuestValue=0;

	RegisterMainQuestByIndex(0);

	SetMainQuestValue(MainQuestValue);

	//int FName 서브퀘스트 배열 똑같이
}

const FQuestDataRow& UQuestManager::GetMainQuestData(FName id) const
{
	return *UQuestData::GetMainQuestTable->FindRow<FQuestDataRow>(id,"!!!UQuestManager::GetMainQuestData=Key Wrong!!!");
}

const FQuestDataRow& UQuestManager::GetSubQuestData(FName id) const
{
	return *UQuestData::GetSubQuestTable->FindRow<FQuestDataRow>(id,"!!!UQuestManager::GetSubQuestData=Key Wrong!!!");
}

void UQuestManager::RegisterMainQuest(FName id)
{
	const FQuestDataRow* QuestDataFound = &GetMainQuestData(id);

	if(!QuestDataFound)
	{
		return;
	}
	
	if(m_MapMainQuestSpec.Contains(QuestDataFound))
	{
		return;//중복
	}

	m_MapMainQuestSpec.Add(QuestDataFound,FQuestSpec(id,*QuestDataFound));

	UQuestLogicBase* QuestLogicCreated = NewObject<UQuestLogicBase>(this,QuestDataFound->m_ClassQuestRegister);

	m_AryMainQuestLogic.Add(QuestLogicCreated);

	FQuestSpec& QuestSpecCreated = m_MapMainQuestSpec[QuestDataFound];

	m_MapMainQuestLogic.Add(&QuestSpecCreated,QuestLogicCreated);

	QuestLogicCreated->RegisterQuest(QuestSpecCreated);
	
	m_OnQuestChanged.Broadcast();

	m_OnQuestAdd.Broadcast(QuestLogicCreated,true);
}

void UQuestManager::RegisterMainQuestByIndex(int index)
{
	FName QuestId = GetMainQuestID(index);
	
	RegisterMainQuest(QuestId);
}

void UQuestManager::RegisterSubQuest(FName id)
{
	const FQuestDataRow* QuestDataFound = &GetSubQuestData(id);
	
	if(m_MapSubQuestSpec.Contains(QuestDataFound))
	{
		return;//중복
	}

	m_MapSubQuestSpec.Add(QuestDataFound,FQuestSpec(id,*QuestDataFound));

	UQuestLogicBase* QuestLogicCreated = NewObject<UQuestLogicBase>(this,QuestDataFound->m_ClassQuestRegister);

	m_ArySubQuestLogic.Add(QuestLogicCreated);

	FQuestSpec& QuestSpecCreated = m_MapSubQuestSpec[QuestDataFound];

	m_MapSubQuestLogic.Add(&QuestSpecCreated,QuestLogicCreated);

	QuestLogicCreated->RegisterQuest(QuestSpecCreated);
	
	m_OnQuestChanged.Broadcast();

	m_OnQuestAdd.Broadcast(QuestLogicCreated,false);
}

void UQuestManager::CompleteMainQuest(FName id)
{
	const FQuestDataRow& QuestDataFound = GetMainQuestData(id);

	const FQuestSpec& QuestSpec = m_MapMainQuestSpec[&QuestDataFound];

	UQuestLogicBase* QuestLogic = m_MapMainQuestLogic[&QuestSpec];

	if(!QuestLogic->CanCompleteQuest())
	{
		return;
	}
	
	QuestLogic->CompleteQuest();
	
	m_OnQuestRemove.Broadcast(QuestLogic,true);

	m_AryMainQuestLogic.Remove(QuestLogic);

	m_MapMainQuestLogic.Remove(&QuestSpec);

	m_MapMainQuestSpec.Remove(&QuestDataFound);

	m_OnQuestChanged.Broadcast();
}

void UQuestManager::CompleteSubQuest(FName id)
{
	const FQuestDataRow& QuestDataFound = GetSubQuestData(id);

	const FQuestSpec& QuestSpec = m_MapSubQuestSpec[&QuestDataFound];

	UQuestLogicBase* QuestLogic = m_MapSubQuestLogic[&QuestSpec];

	if(!QuestLogic->CanCompleteQuest())
	{
		return;
	}

	QuestLogic->CompleteQuest();

	m_OnQuestRemove.Broadcast(QuestLogic,false);
	
	m_ArySubQuestLogic.Remove(QuestLogic);

	m_MapSubQuestLogic.Remove(&QuestSpec);

	m_MapSubQuestSpec.Remove(&QuestDataFound);

	m_OnQuestChanged.Broadcast();
}

void UQuestManager::CompleteQuest(const UQuestLogicBase* quest)
{
	const FQuestSpec& QSpec = quest->GetQuestSpec();
	
	if(m_MapMainQuestLogic.Contains(&QSpec))
	{
		CompleteMainQuest(QSpec.m_QuestID);

		++m_nCurrentMainQuestIndex;
		
		RegisterMainQuestByIndex(m_nCurrentMainQuestIndex);
	}
	else if(m_MapSubQuestLogic.Contains(&QSpec))
	{
		CompleteSubQuest(QSpec.m_QuestID);
	}
}

FName UQuestManager::GetMainQuestID(int index)
{
	return *FString::Printf(TEXT("%.4d"),index);
}

void UQuestManager::SetMainQuestValue(int v)
{
	const FQuestDataRow& QuestDataFound = GetMainQuestData(GetMainQuestID(m_nCurrentMainQuestIndex));

	const FQuestSpec& QuestSpec = m_MapMainQuestSpec[&QuestDataFound];

	UQuestLogicBase* QuestLogic = m_MapMainQuestLogic[&QuestSpec];

	QuestLogic->SetAmount(v);
}

void UQuestManager::SetSubQuestValue(FName id,int v)
{
	const FQuestDataRow& QuestDataFound = GetSubQuestData(id);

	const FQuestSpec& QuestSpec = m_MapSubQuestSpec[&QuestDataFound];

	UQuestLogicBase* QuestLogic = m_MapSubQuestLogic[&QuestSpec];

	QuestLogic->SetAmount(v);
}

UQuestLogicBase* UQuestManager::GetCurrentMainQuest()
{
	const FQuestDataRow& QuestDataFound = GetMainQuestData(GetMainQuestID(m_nCurrentMainQuestIndex));

	if(!m_MapMainQuestSpec.Contains(&QuestDataFound))
	{
		return nullptr;
	}

	const FQuestSpec& QuestSpec = m_MapMainQuestSpec[&QuestDataFound];

	UQuestLogicBase* QuestLogic = m_MapMainQuestLogic[&QuestSpec];
	
	return QuestLogic;
}

const TArray<UQuestLogicBase*>& UQuestManager::GetMainQuests() const
{
	return m_AryMainQuestLogic;
}

const TArray<UQuestLogicBase*>& UQuestManager::GetSubQuests() const
{
	return m_ArySubQuestLogic;
}

//누구든 던전에서 변신 조각을 얻을수 있다
//현질하면 던전 입장횟수가 증가 더 빨리 얻어짐
//하위변신을 열어야 상위 변신 열어짐