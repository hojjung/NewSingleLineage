#include "MyLib.h"

#include "Items/Item_Exe/ItemExecuteBase.h"
#include "Managers/EquipManager.h"
#include "Managers/MyGameInstance.h"
#include "Pawns/MyPlayerPawn.h"
#include "Widgets/HUDs/MapHUD.h"
#include "Widgets/World/Menu/Storage/WidgetStorage.h"

UWorld* UMyLib::GetUWorld()
{
	return UMyGameInstance::Get->GetWorld();
}

AMyPlayerPawn* UMyLib::GetPlayer()
{
	if(!UMyGameInstance::Get->m_Player.Get())
	{
		UMyGameInstance::Get->m_Player = Cast<AMyPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetUWorld(), 0));
	}
	
	return UMyGameInstance::Get->m_Player.Get();
}

UInventory* UMyLib::GetPlayerInven()
{
	return UMyGameInstance::Get->m_Inven;
}

UConstructionManager* UMyLib::GetBuildManager()
{
	return UMyGameInstance::Get->m_BuildManager;
}

AMonsterPawn* UMyLib::GetPickPocketTarget()
{
	return GetCanvas()->GetPickpocketMenu()->GetCurrentTargetPawn();
}

AMyPlayerController* UMyLib::GetPlayerCon()
{
	if(!UMyGameInstance::Get->m_PlayerCon.Get())
	{
		UMyGameInstance::Get->m_PlayerCon = Cast<AMyPlayerController>( UGameplayStatics::GetPlayerController(GetUWorld(),0));
	}
	
	return UMyGameInstance::Get->m_PlayerCon.Get();
}

UWidgetCanvasWorld* UMyLib::GetCanvas()
{
	AMyHUD* HUD = Cast<AMyHUD>(GetPlayerCon()->GetHUD());
	                                                                                                                                       
	return HUD->GetCanvas();
}

UWidgetMapPanel* UMyLib::GetMapCanvas()
{
	AMapHUD *hud = Cast<AMapHUD>(UGameplayStatics::GetPlayerController( GetUWorld(),0)->GetHUD());

	return hud->GetCanvas();
}

float UMyLib::SetFloatPrecision(float TheFloat, int32 Precision)
{
	if (Precision <= 0)
	{
		return roundf(TheFloat);
	}

	Precision = FMath::Clamp(Precision, 1, 10);

	int32 PresRounded = round(FMath::Pow(10, Precision));

	float A = round(TheFloat * PresRounded);
	float B = PresRounded;

	return A / B;
}

FText UMyLib::GetFloatToPercentText(float v)
{
	//v = SetFloatPrecision(v,1);
	
	bool IsMinus = v <0.f;
	
	FTextFormat FormatT;

	FFormatOrderedArguments Args;
	
	if(IsMinus)
	{
		FormatT = FText::FromString("-{0}%");	
	}
	else
	{
		FormatT = FText::FromString("+{0}%");
	}

	Args.Add(SetFloatPrecision(v*100.f,0));

	return FText::Format(FormatT,Args);
}

int UMyLib::GetRectVertIndexPer(const FSlateRect& rect, int maxDivide, const FVector2D& mousePos)
{
	float MinVert = rect.Top;//시작점
	
	float Divide = (rect.Bottom - rect.Top) / maxDivide;//한칸 높이
	
	int MaxIter = maxDivide*2;

	TArray<float>AryVertMinMax;
	
	AryVertMinMax.Init(-1,MaxIter);

	int ResultIndex = 0;

	for(int i=0;i<MaxIter;i+=2)//if 3 than 6, if result 5,0,2,4
	{
		AryVertMinMax[i] = MinVert; 
			
		AryVertMinMax[i+1] = AryVertMinMax[i] + Divide;

		if(mousePos.Y >= AryVertMinMax[i] && mousePos.Y <= AryVertMinMax[i+1])
		{
			break;
		}

		MinVert = AryVertMinMax[i+1];

		ResultIndex++;
	}

	return ResultIndex;

	//divide vertical only
	// Point.X >= Left && Point.X <= Right && Point.Y >= Top && Point.Y <= Bottom;
	//fail contains = return -1?
}

const FItemDataRow& UMyLib::GetItemData(const FName& specID)
{
	const FItemDataRow* FoundItemRow = UItemData::GetItemTable->FindRow<FItemDataRow>(specID,"");

	return *FoundItemRow;
}

const FSkillDataRow& UMyLib::GetSkillData(const FName& specID)
{
	const FSkillDataRow* FoundItemRow = USkillData::GetSkillTable->FindRow<FSkillDataRow>(specID,"");

	return *FoundItemRow;
}

EItemType UMyLib::GetItemType(FName itemKey)
{
	if(itemKey == NAME_None)
	{
		return EItemType::None;
	}
	
	const FItemDataRow& DataRow = GetItemData(itemKey);

	return GetItemType(DataRow);
}

bool UMyLib::IsEquip(FName itemKey)
{
	return UMyLib::GetItemType(itemKey) == EItemType::Equip;
}

bool UMyLib::IsEquip(const FItemDataRow& DataRow)
{
	return UMyLib::GetItemType(DataRow) == EItemType::Equip;
}

EItemType UMyLib::GetItemType(const FItemDataRow& DataRow)
{
	if (DataRow.m_ItemType != EEquipSlotType::None)
	{
		return EItemType::Equip;
	}

	if (DataRow.m_ClassExeItem != nullptr)
	{
		return EItemType::Consume;
	}

	return EItemType::None;
}

UEquipManager* UMyLib::GetEquip()
{
	return UMyGameInstance::Get->m_EquipManager;
}

EEquipSlotType UMyLib::GetEquipItemSlot(const FName& id)
{
	return UMyLib::GetItemData(id).m_ItemType;
}

EEquipSlotType UMyLib::GetEquipItemSlot(const FItemDataRow& data)
{
	return data.m_ItemType;
}

UParticleEffectManager* UMyLib::GetEffectM()
{
	return UMyGameInstance::Get->m_EffectManager;
}

bool UMyLib::CheckAngle(const AActor* center, const AActor* target, float angle)
{
	float PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(angle));

	FVector const OtherLoc = target->GetActorLocation();

	FVector const SensorLoc = center->GetActorLocation();

	FVector const SelfToOther = OtherLoc - SensorLoc;

	FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();

	FVector const MyFacingDir = center->GetActorRotation().Vector();

	return ((SelfToOtherDir | MyFacingDir) >= PeripheralVisionCosine);
}

bool UMyLib::CheckAngle(const FVector centerDir,const FVector centerPos, const AActor* target, float angle)
{
	FVector const OtherLoc = target->GetActorLocation();
	
	const FVector& ActorDirection = (OtherLoc - centerPos).GetSafeNormal();
	
	const float DotProduct = FVector::DotProduct(ActorDirection, centerDir);

	const float LowerLimit =  FMath::Cos(FMath::DegreesToRadians(angle / 2));
	
	constexpr float UpperLimit = 1.0f;
			
	return DotProduct >= LowerLimit && DotProduct <= UpperLimit;
}

void UMyLib::ShowSkillFail(FString reason)
{
//	GetCanvas()->ShowSkillFail(reason);
}

bool UMyLib::BoxOverlapActors(UObject* WorldContextObject,FRotator rot,const FVector BoxPos, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
{
	OutActors.Empty();

	TArray<UPrimitiveComponent*> OverlapComponents;
	bool bOverlapped = BoxOverlapComponents(WorldContextObject,rot,BoxPos, BoxExtent, ObjectTypes, NULL, ActorsToIgnore, OverlapComponents);
	if (bOverlapped)
	{
		UKismetSystemLibrary::GetActorListFromComponentList(OverlapComponents, ActorClassFilter, OutActors);
	}

	return (OutActors.Num() > 0);
}

bool UMyLib::BoxOverlapComponents(UObject* WorldContextObject,FRotator rot,const FVector BoxPos, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents)
{
	OutComponents.Empty();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(BoxOverlapComponents), false);
	Params.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;

	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel & Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		ObjectParams.AddObjectTypesToQuery(Channel);
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr)
	{
		World->OverlapMultiByObjectType(Overlaps, BoxPos, rot.Quaternion(), ObjectParams, FCollisionShape::MakeBox(BoxExtent), Params);
	}

	for (int32 OverlapIdx=0; OverlapIdx<Overlaps.Num(); ++OverlapIdx)
	{
		FOverlapResult const& O = Overlaps[OverlapIdx];
		if (O.Component.IsValid())
		{ 
			if ( !ComponentClassFilter || O.Component.Get()->IsA(ComponentClassFilter) )
			{
				OutComponents.Add(O.Component.Get());
			}
		}
	}

	return (OutComponents.Num() > 0);
}

bool UMyLib::SphereOverlapActors(UObject* WorldContextObject,FRotator Rot ,const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
{
	OutActors.Empty();

	TArray<UPrimitiveComponent*> OverlapComponents;
	bool bOverlapped = SphereOverlapComponents(WorldContextObject,Rot, SpherePos, SphereRadius, ObjectTypes, NULL, ActorsToIgnore, OverlapComponents);
	if (bOverlapped)
	{
		UKismetSystemLibrary::GetActorListFromComponentList(OverlapComponents, ActorClassFilter, OutActors);
	}

	return (OutActors.Num() > 0);
}

bool UMyLib::SphereOverlapComponents(UObject* WorldContextObject,FRotator Rot , const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents)
{
	OutComponents.Empty();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SphereOverlapComponents), false);
	Params.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;

	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel & Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		ObjectParams.AddObjectTypesToQuery(Channel);
	}


	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(World != nullptr)
	{
		World->OverlapMultiByObjectType(Overlaps, SpherePos, Rot.Quaternion(), ObjectParams, FCollisionShape::MakeSphere(SphereRadius), Params);
	}

	for (int32 OverlapIdx=0; OverlapIdx<Overlaps.Num(); ++OverlapIdx)
	{
		FOverlapResult const& O = Overlaps[OverlapIdx];
		if (O.Component.IsValid())
		{ 
			if ( !ComponentClassFilter || O.Component.Get()->IsA(ComponentClassFilter) )
			{
				OutComponents.Add(O.Component.Get());
			}
		}
	}

	return (OutComponents.Num() > 0);
}

void UMyLib::SetBtnTint(UButton* btn, FLinearColor colrWant)
{
	btn->SetColorAndOpacity(colrWant);
	btn->WidgetStyle.Normal.TintColor = colrWant;
	btn->WidgetStyle.Pressed.TintColor = colrWant;
	btn->WidgetStyle.Hovered.TintColor = colrWant;
}

int UMyLib::GetRequireCollecLevel(const FName& collecID, int index)
{
	return UItemCollectionTable::GetItemCollecTable->FindRow<FItemCollecRow>(collecID,"")->m_AryItems[index].m_nEnchantLv;
}

bool UMyLib::IsCollecItemEquip(const FName& collecID, int index)
{
	const FName ItemKey = UItemCollectionTable::GetItemCollecTable->FindRow<FItemCollecRow>(collecID,"")->m_AryItems[index].m_Item.RowName;

	return UMyLib::IsEquip(ItemKey);
}

int UMyLib::GetItemCountAllInven(const FName& id, int stlv)
{
	int Sum = UMyLib::GetPlayerInven()->GetItemCount(id, stlv);
	
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;
	
	if(Equip->GetBag())
	{
		Sum += Equip->GetBag()->GetItemCount(id, stlv);
	}
	if(Equip->GetBelt())
	{
		Sum += Equip->GetBelt()->GetItemCount(id, stlv);
	}
	return Sum;	
}

FItemSpec* UMyLib::FindItemAllInven(const FName& id, int stlv)
{
	UInventory* outInven;
	return FindItemAllInven(id,stlv,outInven);
}

FItemSpec* UMyLib::FindItemAllInven(const FName& id, int stlv, UInventory*& outInven)
{
	FItemSpec* ItemFound = nullptr;

	ItemFound = UMyLib::GetPlayerInven()->FindItem(id, stlv);
	if(ItemFound)
	{
		outInven =  UMyLib::GetPlayerInven();
		return  ItemFound;
	}
	
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;
	
	if(Equip->GetBag())
	{
		ItemFound = Equip->GetBag()->FindItem(id, stlv);
		if(ItemFound)
		{
			outInven =  Equip->GetBag();
			return ItemFound;
		}
	}
	if(Equip->GetBelt())
	{
		ItemFound = Equip->GetBelt()->FindItem(id, stlv);
		if(ItemFound)
		{
			outInven =  Equip->GetBelt();
			return ItemFound;
		}
	}
	
	return nullptr;
}

bool UMyLib::IsTestMode()
{
	bool IsTestMode = false;
	GConfig->GetBool(
		TEXT("/Script/MyJrpgEditor.MyDeveloperSettings"),
		TEXT("m_bIsTestMode"),
		IsTestMode,
		GGameIni
	);
	return IsTestMode;
}

void UMyLib::ReduceDurability(const FItemSpec& item_spec, int amount)
{
	if(UMyLib::GetPlayerInven()->ReduceDurability(item_spec, amount))
	{
		return;
	}
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;
	if(&Equip->GetEquipItem(EEquipSlotType::Weapon) == &item_spec)
	{
		Equip->ReduceDurability(EEquipSlotType::Weapon, amount);
		return;
	}
	if(Equip->GetBag() &&Equip->GetBag()->ReduceDurability(item_spec, amount))
	{
		return;
	}
	if(Equip->GetBelt() &&Equip->GetBelt()->ReduceDurability(item_spec, amount))
	{
		return;
	}
}

const FStatGroup& UMyLib::GetItemStatData(const FName& id)
{
	return UMyLib::GetItemData(id).m_EquipStats;
}

bool UMyLib::HasSpaceAllInven(FItemSpec& item)
{
	if (UMyLib::GetPlayerInven()->HasSpace(item))
	{
		return true;
	}
	
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;
	
	if(Equip->GetBag())
	{
		if (Equip->GetBag()->HasSpace(item))
		{
			return true;
		}
	}
	if(Equip->GetBelt())
	{
		if (Equip->GetBelt()->HasSpace(item))
		{
			return true;
		}
	}
	return false;	
}

bool UMyLib::RemoveItemAll(const FName& id, int stLv)
{
	if (UMyLib::GetPlayerInven()->RemoveItem(id, stLv))
	{
		return true;
	}
	
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;
	
	if(Equip->GetBag())
	{
		if (Equip->GetBag()->RemoveItem(id, stLv))
		{
			return true;
		}
	}
	if(Equip->GetBelt())
	{
		if (Equip->GetBelt()->RemoveItem(id, stLv))
		{
			return true;
		}
	}
	return false;	
}

bool UMyLib::AddItemAll(FItemSpec& items, bool newItem)
{
	UEquipManager* Equip = UMyGameInstance::Get->m_EquipManager;

	return Equip->AddItem(items, newItem);
}

void UMyLib::PrintInfoText(const FString& string)
{
	ABaseHUD *hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController( GetUWorld(),0)->GetHUD());
	
	hud->PrintInfoText(string);
}

void UMyLib::PrintInfoText(FText&& tt)
{
	PrintInfoText(tt.ToString());
}

void UMyLib::OpenItemInfo(const FItemDataRow& itemData)
{
	ABaseHUD *hud = Cast<ABaseHUD>(UGameplayStatics::GetPlayerController( GetUWorld(),0)->GetHUD());

	hud->OpenItemInfo(itemData);
}
