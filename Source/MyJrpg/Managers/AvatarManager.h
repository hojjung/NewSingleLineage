// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "UObject/NoExportTypes.h"
#include "AvatarManager.generated.h"

class APreviewActor;
/**
 * 
 */
UCLASS()
class MYJRPG_API UAvatarManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkinChanged, const FPlayerUnitEntityRow&);

	FOnSkinChanged m_OnSkinChanged;
	UPROPERTY()
	APreviewActor* m_PreviewActor;
public:
	void CreatePreviewActor();

	void EquipSkin(const FName& id);
	
	void SetIsTouched(bool b);
	
	void RotatePawn(float delta_x);
};
