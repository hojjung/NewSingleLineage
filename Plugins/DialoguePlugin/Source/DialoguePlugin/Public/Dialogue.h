// Copyright Underflow Studios 2017
#pragma once
#include "Runtime/Engine/Classes/Engine/DataAsset.h"
#include "CoreMinimal.h"
#include "Dialogue.generated.h"

class UGameInstance;
class UDialogue;
// Extend this class to add custom events
UCLASS(Blueprintable, BlueprintType, abstract, EditInlineNew, HideCategories = ("DoNotShow"), CollapseCategories, AutoExpandCategories = ("Default"))
class DIALOGUEPLUGIN_API UDialogueEvents : public UObject
{
	GENERATED_BODY()
	
public:
	UDialogueEvents();

	virtual void RecieveEventTriggered(APlayerController* ConsideringPlayer, AActor* NPCActor);

	virtual class UWorld* GetWorld() const override;
};

// Extend this class to add custom conditions.
UCLASS(Blueprintable, BlueprintType, abstract, EditInlineNew, HideCategories = ("DoNotShow"), CollapseCategories, AutoExpandCategories = ("Default"))
class DIALOGUEPLUGIN_API UDialogueConditions : public UObject
{
	GENERATED_BODY()
	
public:
	UDialogueConditions();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue Conditions")
	bool IsConditionMet(APlayerController* ConsideringPlayer, AActor* NPCActor);

	UFUNCTION()
	virtual bool IsConditionMet_Internal(APlayerController* ConsideringPlayer, AActor* NPCActor) { return true; }

	virtual class UWorld* GetWorld() const override;
};

// A particular type of condition that nests normal conditions and returns true if at least one of them is true.
UCLASS(NotBlueprintable, BlueprintType, EditInlineNew, HideCategories = ("DoNotShow"), CollapseCategories, AutoExpandCategories = ("Default"))
class DIALOGUEPLUGIN_API UOrCondition : public UDialogueConditions
{
	GENERATED_BODY()

public:
	UOrCondition(){ }

	UPROPERTY(Instanced, EditDefaultsOnly, Category = "Dialogue Conditions")
	TArray<UDialogueConditions*> OrConditions;

	virtual bool IsConditionMet_Internal(APlayerController* ConsideringPlayer, AActor* NPCActor) override
	{
		for (auto & cond : OrConditions)
		{
			if (cond->IsConditionMet(ConsideringPlayer, NPCActor))
				return true;
		}
		return false;
	}
};

// A particular type of condition that nests normal conditions and returns true if at least one of them is true.
UCLASS(NotBlueprintable, BlueprintType, EditInlineNew, HideCategories = ("DoNotShow"), CollapseCategories, AutoExpandCategories = ("Default"))
class DIALOGUEPLUGIN_API UAndCondition : public UDialogueConditions
{
	GENERATED_BODY()

public:
	UAndCondition(){ }

	UPROPERTY(Instanced, EditDefaultsOnly, Category = "Dialogue Conditions")
		TArray<UDialogueConditions*> AndConditions;

	virtual bool IsConditionMet_Internal(APlayerController* ConsideringPlayer, AActor* NPCActor) override
	{
		for (auto & cond : AndConditions)
		{
			if (!cond->IsConditionMet(ConsideringPlayer, NPCActor))
				return false;
		}
		return true;
	}
};


USTRUCT(BlueprintType)
struct FDialogueNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	FName m_SpeakerIDNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	int32 m_nID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	bool m_bIsPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node", meta=( MultiLine="true" ))
	FText m_Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	TArray<int32> m_AryLinks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	FVector2D m_Coordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	FName m_QuestID;

	UPROPERTY(Instanced, EditDefaultsOnly, Category = "Dialogue Node")
	TArray<UDialogueEvents*> m_AryEvents;

	UPROPERTY(Instanced, EditDefaultsOnly, Category = "Dialogue Node")
	TArray<UDialogueConditions*> m_AryConditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	class USoundBase* m_Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Node")
	class UDialogueWave* m_DialogueWave = nullptr;
};

UCLASS(Blueprintable, BlueprintType)
class DIALOGUEPLUGIN_API UDialogue : public UDataAsset
{
	GENERATED_BODY()

public:
	UDialogue(const FObjectInitializer& ObjectInitializer);
	
public:
	TWeakObjectPtr<UGameInstance> m_PersistentGameInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	bool m_bDisplayIdleSplines = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FName m_SpeakerID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	TArray<FDialogueNode> m_AryData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue)
	int32 m_nNextNodeId;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	int32 m_nCurrentNodeId;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	bool m_bIsLinking;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FVector2D m_LinkingCoords;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	int32 m_nLinkingFromIndex;
	
public:
	static void CallFunctionByName(UObject* Object, FString FunctionName);
	
	const FDialogueNode* GetFirstNode();

	TArray<const FDialogueNode*> GetNextNodes(const FDialogueNode& Node);

	void AssignPersistentOuter(class UGameInstance * inGameInstance);

	virtual UWorld* GetWorld() const override;	

	const FDialogueNode* GetNodeById(int32 id, int32 &index);
	
	const FDialogueNode* GetNodeById(int32 id);
	
	void CleanOuter();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Dialogue"), GetFName());
	}
};
