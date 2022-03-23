// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "Actors/MyPlayerController.h"
#include "DataTables/ItemData.h"
#include "DataTables/SkillData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Managers/ParticleEffectManager.h"
#include "Widgets/HUDs/MyHUD.h"
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

	static UInventory* GetPlayerStorage();
	
	static AMyPlayerController* GetPlayerCon();

	static UWidgetCanvasWorld* GetCanvas();

	//static UWidgetCanvasGameLevel* GetMainWidget();

	UFUNCTION(BlueprintCallable)
	static float SetFloatPrecision(float TheFloat, int32 Precision=1);

	static FText GetFloatToPercentText(float v);

	static int GetRectVertIndexPer(const FSlateRect& rect,int maxDivide/*if 0,1,2 than 3*/,const FVector2D& mousePos);

	static const FItemDataRow& GetItemData(const FName& specID);

	static const FSkillDataRow& GetSkillData(const FName& specID);

	static EItemType GetItemType(FName itemKey);

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

	static UParticleEffectManager* GetEffectM();

	static bool CheckAngle(const AActor* center, const AActor* target, float angle);

	static void ShowSkillFail(FString reason);

	static bool BoxOverlapActors(UObject* WorldContextObject, FRotator rot, const FVector BoxPos, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors);
	
	static bool BoxOverlapComponents(UObject* WorldContextObject, FRotator rot, const FVector BoxPos, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents);

	static bool SphereOverlapActors(UObject* WorldContextObject,FRotator Rot ,const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors);

	static bool SphereOverlapComponents(UObject* WorldContextObject,FRotator Rot , const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents);

	static void SetBtnTint(UButton* btn, FLinearColor colrWant);

	static FName GenerateEquipItemHashKey(const FName& id, const void *ptr);

	static FName GetEquipIDFromHashID(const FName& hash_id);
};
