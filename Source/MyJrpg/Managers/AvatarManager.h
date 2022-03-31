// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Interfaces/PreviewProxy.h"
#include "UObject/NoExportTypes.h"
#include "AvatarManager.generated.h"

class APreviewActor;
/**
 * 
 */
UCLASS()
class MYJRPG_API UAvatarManager : public UObject, public IPreviewProxy
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnSkinChanged);

	FOnSkinChanged m_OnSkinChanged;
	
protected:
	UPROPERTY()
	APreviewActor* m_PreviewActor;

	TArray<FPlayerUnitEntityRow*> m_ArySkins;

	const FPlayerUnitEntityRow* m_CrntSkin;
	
public:
	void Init();

	const TArray<FPlayerUnitEntityRow*>& GetAllSkins() const;
	
	void CreatePreviewActor();

	void EquipSkin(const FName& id);

	void EquipSkin(const FPlayerUnitEntityRow& selected);
	
	void ShowPreviewSkin(const FPlayerUnitEntityRow& selected);

	virtual void SetIsTouched(bool b) override;

	virtual void RotatePawn(float delta_x) override;

	void ShowPawn();

	void HidePawn();

	const FPlayerUnitEntityRow* GetCrntSkin() const;
};
