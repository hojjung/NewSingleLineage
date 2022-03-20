#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/UnitEntityData.h"

#include "MySaveGame.generated.h"

UCLASS()
class MYJRPG_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	FTimespan m_PlayTime;
	UPROPERTY(EditAnywhere)
	FString m_PlayerName;
	UPROPERTY(EditAnywhere)
	FString m_ChapterName;
	UPROPERTY(EditAnywhere)
	FString m_ZoneName;
	UPROPERTY(EditAnywhere)
	TArray<FString> m_AryEquippedAllies;//0 is Player
	UPROPERTY(EditAnywhere)
	TArray<FVector> m_AryEquippedAllyPosition;
	UPROPERTY(EditAnywhere)
	TArray<FRotator> m_AryEquippedAllyRotation;
	UPROPERTY(EditAnywhere)
	TArray<FUnitSpec> m_AryTotalAlly;
	UPROPERTY(EditAnywhere)
	TArray<FItemSpec> m_AryInventory;
	
public:
	FString GetSaveName();

	FString GetPlayTime();

	void SetSaveGameDefault(FTimespan& totalPlayTime,FString& playerName,FString& chapterName,FString& zoneName);

	const TArray<FUnitSpec>& GetTotalAryAllies() const;

	const TArray<FString>& GetEquipAryAllies() const;

	const TArray<FVector>& GetEquipAryLocations() const;

	const TArray<FRotator>& GetEquipAryRotations() const;

	const TArray<FItemSpec>& GetInventory() const;

	FName GetZoneName();

	FString GetChapterName();

	const FString& GetPlayerName();
	
	void SetSaveEquippedAllies(const TArray<FString>& equipAllies,const TArray<FVector>& aryLocations,const TArray<FRotator>& aryRotations);
	
	void SetSaveTotalAllies(const TArray<FUnitSpec>& arrayUnitSpecs);
};
