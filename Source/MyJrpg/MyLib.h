// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "Actors/MyPlayerController.h"
#include "Actors/Field/Gather/TreeBase.h"
#include "DataTables/ItemData.h"
#include "DataTables/SkillData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Managers/ConstructionManager.h"
#include "Managers/CraftManager.h"
#include "Managers/ParticleEffectManager.h"
#include "Widgets/HUDs/MyHUD.h"
#include "Widgets/MapSelect/WidgetMapPanel.h"
#include "MyLib.generated.h"

class UInventory;
class AMyPlayerPawn;
class UEquipManager;
/**
 * 
 */
UCLASS()
class MYJRPG_API UMyLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UWorld* GetUWorld();
	
	static AMyPlayerPawn* GetPlayer();

	static UInventory* GetPlayerInven();

	static UConstructionManager* GetBuildManager();

	static AMonsterPawn* GetPickPocketTarget();
	
	static AMyPlayerController* GetPlayerCon();

	static UWidgetCanvasWorld* GetCanvas();

	static UWidgetMapPanel* GetMapCanvas();

	//static UWidgetCanvasGameLevel* GetMainWidget();

	UFUNCTION(BlueprintCallable)
	static float SetFloatPrecision(float TheFloat, int32 Precision=1);

	static FText GetFloatToPercentText(float v);

	static int GetRectVertIndexPer(const FSlateRect& rect,int maxDivide/*if 0,1,2 than 3*/,const FVector2D& mousePos);

	static const FItemDataRow& GetItemData(const FName& specID);

	static const FSkillDataRow& GetSkillData(const FName& specID);

	static EItemType GetItemType(FName itemKey);

	static bool IsEquip(FName itemKey);

	static bool IsEquip(const FItemDataRow& DataRow);

	static EItemType GetItemType(const FItemDataRow&);

	static UNavigationSystemV1* GetNavSys()
	{
		return FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetUWorld());	
	}

	static FString GetPrimaryGPUBrand() {
		return FGenericPlatformMisc::GetPrimaryGPUBrand();
	}

	static UEquipManager* GetEquip();

	static EEquipSlotType GetEquipItemSlot(const FName& id);

	static EEquipSlotType GetEquipItemSlot(const FItemDataRow& data);

	static UParticleEffectManager* GetEffectM();

	static bool CheckAngle(const AActor* center, const AActor* target, float angle);

	static bool CheckAngle(const FVector centerDir,const FVector centerPos, const AActor* target, float angle);

	static void ShowSkillFail(FString reason);

	static bool BoxOverlapActors(UObject* WorldContextObject, FRotator rot, const FVector BoxPos, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors);
	
	static bool BoxOverlapComponents(UObject* WorldContextObject, FRotator rot, const FVector BoxPos, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents);

	static bool SphereOverlapActors(UObject* WorldContextObject,FRotator Rot ,const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors);

	static bool SphereOverlapComponents(UObject* WorldContextObject,FRotator Rot , const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents);

	static void SetBtnTint(UButton* btn, FLinearColor colrWant);

	static int GetRequireCollecLevel(const FName& collecID, int index);

	static bool IsCollecItemEquip(const FName& collecID, int index);

	static int GetItemCountAllInven(const FName& id, int stlv);
	
	static FItemSpec* FindItemAllInven(const FName& id, int stlv);

	static FItemSpec* FindItemAllInven(const FName& id, int stlv,UInventory*& outInven);
	
	static bool IsTestMode();
	
	static void ReduceDurability(const FItemSpec& item_spec, int amount);
	
	static const FStatGroup& GetItemStatData(const FName& id);
	
	static bool HasSpaceAllInven(FItemSpec& item);
	
	static bool RemoveItemAll(const FName& id, int stLv);
	
	static bool AddItemAll(FItemSpec& items, bool newItem);

	static void PrintErrorText(const FString& string);

	static void PrintErrorText(FText&& tt);
	
	static void OpenItemInfo(const FItemDataRow& itemData);
};
