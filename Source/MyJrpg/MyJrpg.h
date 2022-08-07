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

	static const float HERO_DEFAULT_SPEED = 420.f;

	static const float HUNGER_DELAY = 2.f;
	
	static const float HUNGER_DAMAGE = 0.5f;

	static const int INVEN_SIZE = 10;//42//14

	static const int STORAGE_SIZE = 20;

	static const int ENCHANT_MAX = 20;

	static const int KARMA_MAX = 100;
	
	static const int KARMA_MIN = -100;

	static const int KARMA_FRIEND = 50;
	
	static const int KARMA_FOE = -50;

	static const int MOB_INVEN = 10;

	static const int GRID_COUNT = 15;

	static const float GRID_SIZE = 333.3f;//??188
};

//(X=0.885000,Y=0.885000,Z=0.885000) =타일 스캐일
//그리드 20칸
//평원 크기 50
#define PRINTFSCREEN(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.5, FColor::Green, *FString::Printf(TEXT(Format), ##__VA_ARGS__),true)
#define PRINTF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)) PRINTFSCREEN(Format, ##__VA_ARGS__)
#define PRINT3DF(Location,Format,...) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *FString::Printf(TEXT(Format), ##__VA_ARGS__), nullptr,  FColor::Orange, 0, false);
#define PRINT3D(Location,fString) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *fString, nullptr,  FColor::Orange, 0, false);
#define PRINT3DGreen(Location,fString) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *fString, nullptr,  FColor::Green, 0, false);

DECLARE_DELEGATE(FVoidVoid);

UENUM(BlueprintType)
enum class ETextType: uint8
{
	NormalRight,
	PlDmg,
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
	Bag,
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
	None,
	Inven,
	Storage,
	ShopBuy,
	ShopSell,
	Enchant,
	PickPocketTarget,
	PickPocketPl
};

class DeleterNot
{
public:
	void operator()(void*) {}
};

enum class EKarma
{
	Neutral,
	Friendly,
	Hate
};

template<typename T>
  TSharedPtr<T> MakeSharedUObject(T* obj)
{
	TSharedRef< TStrongObjectPtr<T> > SharedRefToStrongObjPtr = MakeShared< TStrongObjectPtr<T> >(obj);
	return TSharedPtr<T>(SharedRefToStrongObjPtr, SharedRefToStrongObjPtr->Get());
}


UENUM()
enum class EBuildType :uint8
{
	Foundation,
	Wall,
	Furniture,
	Field
};

UENUM(BlueprintType)
enum class EStanceType :uint8
{
	None,
	OneSword,
	SwordShield,
	Dual,
	TwoSword,
	Spear,
	Bow,
	Pistol,
	Rifle,
	OneMagic,
	Length
};