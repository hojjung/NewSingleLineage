#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//
//
#include "Engine/Engine.h"
#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
//
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture.h"
#include "Engine/Font.h"
//
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
//
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//
#include "Blueprint/WidgetBlueprintLibrary.h"
//
#include "DrawDebugHelpers.h"
//
#include "Components/SceneComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Net/UnrealNetwork.h"

//
MYJRPG_API DECLARE_LOG_CATEGORY_EXTERN(MyJrpg, Log, All);

namespace FGlobalVariable
{
	static const int LEVEL_MAX = 100;

	static const float HERO_DEFAULT_SPEED = 460.f;

	static const int INVEN_MAXSTACK = 9999;//9999
	
	static const int INVEN_SIZE = 32;//42

	static const int STORAGE_SIZE = 24;

	static const int QUICKSLOT_MAX = 5;
};

#define PRINTFSCREEN(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.5, FColor::Green, *FString::Printf(TEXT(Format), ##__VA_ARGS__),true)
#define PRINTF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)) PRINTFSCREEN(Format, ##__VA_ARGS__)
#define PRINT3DF(Location,Format,...) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *FString::Printf(TEXT(Format), ##__VA_ARGS__), nullptr,  FColor::Orange, 0, false);
#define PRINT3D(Location,fString) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *fString, nullptr,  FColor::Orange, 0, false);
#define PRINT3DGreen(Location,fString) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *fString, nullptr,  FColor::Green, 0, false);

DECLARE_MULTICAST_DELEGATE(FVoidVoid);

UENUM(BlueprintType)
enum class ETextType: uint8
{
	NormalRight,
	CritcalRight,
	Miss,
	PlayerTookHeal,
	Immune,
	Length UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipSlotType:uint8
{
	None,
	Weapon,
	Head,
	Torso,
	Hand,
	Leg,
	Ring,
	Neckless,
	EarRing,
	Belt,
	Length UMETA(Hidden)
};

enum class EItemType
{
	None,
	misc,
	Consume,
	Equip
};

enum class EPanelType
{
	Inven,
	StorageDeposit,
	StorageWithdraw,
	ShopBuy,
	ShopSell,
	Enchant
};

class DeleterNot
{
public:
	void operator()(void*) {}
};
