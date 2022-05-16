#include "MyLib.h"

#include "Items/ItemExecuteBase.h"
#include "Managers/MyGameInstance.h"
#include "Pawns/MyPlayerPawn.h"
#include "Widgets/World/Menu/Storage/WidgetStorage.h"

UWorld* UMyLib::GetUWorld()
{
	return UMyGameInstance::Get->GetWorld();
}

AMyPlayerPawn* UMyLib::GetPlayer()
{
	if(!UMyGameInstance::Get->m_Player)
	{
		UMyGameInstance::Get->m_Player = Cast<AMyPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetUWorld(), 0));
	}
	
	return UMyGameInstance::Get->m_Player;
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
	if(!UMyGameInstance::Get->m_PlayerCon)
	{
		UMyGameInstance::Get->m_PlayerCon = Cast<AMyPlayerController>( UGameplayStatics::GetPlayerController(GetUWorld(),0));
	}
	
	return UMyGameInstance::Get->m_PlayerCon;
}

UWidgetCanvasWorld* UMyLib::GetCanvas()
{
	AMyHUD* HUD = Cast<AMyHUD>(GetPlayerCon()->GetHUD());
                                                                                                                                       
	return HUD->GetCanvas();                                                                                                               
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

FStatGroup UMyLib::GetItemStatData(const FName& specID)
{
	// int Level = UMyLib::GetPlayerInven()->GetItemLevel(specID);
	//
	// const FItemDataRow& ItemData = GetItemData(specID);
	//
	// if(Level == 0)
	// {
	// 	return ItemData.m_EquipStats;
	// }
	//
	// return ItemData.m_EquipStats + (ItemData.m_EnchantStats * Level);
	return FStatGroup(); 
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

UInventory* UMyLib::FindEquipItem(const FName& id)
{
	// if(UMyGameInstance::Get->m_Inven->FindEquipItem(id))
	// {
	// 	return UMyGameInstance::Get->m_Inven;
	// }
	//
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	if(Storage->FindEquipItem(id))
	// 	{
	// 		return Storage;
	// 	}
	// }
	return nullptr;
}

UInventory* UMyLib::FindEquipItem(const FName& id,const FName** gidItem)
{
	// *gidItem = UMyGameInstance::Get->m_Inven->FindEquipItem(id);
	//
	// if(*gidItem)
	// {
	// 	return UMyGameInstance::Get->m_Inven;
	// }
	//
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	*gidItem = Storage->FindEquipItem(id);
	// 	
	// 	if(*gidItem)
	// 	{
	// 		return Storage;
	// 	}
	// }
	return nullptr;
}

UInventory* UMyLib::FindEquipItem(const FName& id, int lv,const FName** gidItem)
{
	// *gidItem = UMyGameInstance::Get->m_Inven->FindEquipItem(id, lv);
	//
	//  if(*gidItem)
	//  {
	//  	return UMyGameInstance::Get->m_Inven;
	//  }
	//
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	*gidItem = Storage->FindEquipItem(id, lv);
	// 	
	// 	if(*gidItem)
	// 	{
	// 		return Storage;
	// 	}
	// }
	return nullptr;
}

UInventory* UMyLib::FindEquipItem(const FName& id, int lv)
{
	// if(UMyGameInstance::Get->m_Inven->FindEquipItem(id, lv))
	// {
	// 	return UMyGameInstance::Get->m_Inven;
	// }
	//
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	if(Storage->FindEquipItem(id, lv))
	// 	{
	// 		return Storage;
	// 	}
	// }
	return nullptr;
}

UInventory* UMyLib::FindMiscItem(const FName& id)
{
	// if(UMyGameInstance::Get->m_Inven->FindMisItem(id))
	// {
	// 	return UMyGameInstance::Get->m_Inven;
	// }
	//
	// for(auto Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	if(Storage->FindMisItem(id))
	// 	{
	// 		return Storage;
	// 	}
	// }
	return nullptr;
}

bool UMyLib::FindMiscItem(const FName& id, int count)
{
	// int InvenStack = UMyGameInstance::Get->m_Inven->GetItemStack(id);
	//
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	InvenStack += Storage->GetItemStack(id);
	// }
	//
	// return InvenStack >= count;
	return false;
}

int UMyLib::GetMiscTotalCount(const FName& id)
{
	// int InvenStack = UMyGameInstance::Get->m_Inven->GetItemStack(id);
	//
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	InvenStack += Storage->GetItemStack(id);
	// }
	//
	// return InvenStack;
	return 0;
}

int UMyLib::GetEquipTotalCount(const FName& oID, int level)
{
	// int EquipCount = UMyGameInstance::Get->m_Inven->GetEquipItemCount(oID,level);
	//
	// for(auto Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	EquipCount += Storage->GetEquipItemCount(oID,level);
	// }
	//
	// return EquipCount;
	return 0;
}

void UMyLib::RemoveMiscItem(const FName& id, int count)
{
	// int InvenStack = UMyGameInstance::Get->m_Inven->GetItemStack(id);
	//
	// if(InvenStack >= count)
	// {
	// 	UMyGameInstance::Get->m_Inven->RemoveItem(id, count);
	// 	return;
	// }
	// else
	// {
	// 	if(InvenStack > 0)
	// 	{
	// 		UMyGameInstance::Get->m_Inven->RemoveItem(id, InvenStack);
	// 	}
	// 	count -= InvenStack;
	// }
	// for(UInventory* Storage : UMyGameInstance::Get->GetStorages())
	// {
	// 	InvenStack = Storage->GetItemStack(id);
	//
	// 	if(InvenStack >= count)
	// 	{
	// 		Storage->RemoveItem(id, count);
	// 		return;
	// 	}
	// 	else
	// 	{
	// 		if(InvenStack > 0)
	// 		{
	// 			Storage->RemoveItem(id, InvenStack);
	// 		}
	// 		count -= InvenStack;
	// 	}
	// }
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

TArray<UInventory*>& UMyLib::GetPlayerStorage()
{
	return UMyGameInstance::Get->GetStorages();
}
