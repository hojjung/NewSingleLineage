#include "Dialogue.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Engine/GameInstance.h"
#include "DialoguePluginPrivatePCH.h"



UDialogue::UDialogue(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UDialogue::AssignPersistentOuter(UGameInstance * inGameInstance)
{
	m_PersistentGameInstance = inGameInstance;
}

UWorld* UDialogue::GetWorld() const
{	
	if (m_PersistentGameInstance.Get())
	{
		return m_PersistentGameInstance->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

const FDialogueNode* UDialogue::GetNodeById(int32 id, int32 & index)
{
	index = -1;

	int i = 0;
	for (const FDialogueNode& FoundNode : m_AryData)
	{
		if (FoundNode.m_nID == id)
		{
			index = i;
			return &FoundNode;
		}
		i++;
	}

	return nullptr;
}

const FDialogueNode* UDialogue::GetNodeById(int32 id)
{
	int32 index;
	return GetNodeById(id, index);
}

void UDialogue::CleanOuter()
{
	m_PersistentGameInstance = nullptr;
}

const FDialogueNode* UDialogue::GetFirstNode()
{
	const FDialogueNode* StartNode = GetNodeById(0);

	if (StartNode->m_AryLinks.Num() > 0)
	{
		return GetNodeById(StartNode->m_AryLinks[0]);
	}

	return nullptr;
}

TArray<const FDialogueNode*> UDialogue::GetNextNodes(const FDialogueNode&  Node)
{
	TArray<const FDialogueNode*> Output;

	for (int32 foundindex : Node.m_AryLinks)
	{
		Output.Add(GetNodeById(foundindex));
	}

	return Output;
}

void UDialogue::CallFunctionByName(UObject* Object, FString FunctionName)
{
	FOutputDeviceNull ar;
	Object->CallFunctionByNameWithArguments(*FunctionName, ar, NULL, true);
}

UDialogueConditions::UDialogueConditions() 
{

}

UWorld* UDialogueConditions::GetWorld() const
{
	UDialogue * outerDialogue = GetTypedOuter<UDialogue>();

	if (outerDialogue)
	{
		return outerDialogue->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

UDialogueEvents::UDialogueEvents()
{

}

void UDialogueEvents::RecieveEventTriggered(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	
}


bool UDialogueConditions::IsConditionMet_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	return IsConditionMet_Internal(ConsideringPlayer, NPCActor);
}

UWorld* UDialogueEvents::GetWorld() const
{
	UDialogue * outerDialogue = GetTypedOuter<UDialogue>();

	if (outerDialogue)
	{
		return outerDialogue->GetWorld();
	}
	else
	{
		return nullptr;
	}
}