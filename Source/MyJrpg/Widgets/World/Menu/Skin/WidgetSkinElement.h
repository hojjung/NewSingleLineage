// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Managers/AvatarManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetSkinElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetSkinElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<UAvatarManager> m_AvatarManager;

	const FPlayerUnitEntityRow* m_Skin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Ele;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_Selected;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgWeapon;
	
public:
	void Init(const FPlayerUnitEntityRow& pl);
	
	void UpdateEle();
};
