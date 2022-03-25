// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageText.h"
#include "WidgetEnchantOption.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Managers/EnchantManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetEnchantBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetEnchantBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemElement* m_TargetItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemElement* m_TargetMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnchant;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarEnchantLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_Inven; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetImageText* m_TextEnchantCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEnchantOption* m_StatLevel;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetEnchantOption> m_ClassOption;
	UPROPERTY(EditDefaultsOnly)
	TArray<UWidgetEnchantOption*> m_AryOptions;
	
protected:
	virtual void NativeOnInitialized() override;

	void Update();
	
	void UpdateIcons(const FName& target, const FName& mat, int level);
	
	void UpdateInfoTexts(const UEnchantManager* Enchant, const FName& target, const FName& mat, int level);
	
	void UpdateBeforeAfter(const FName& target, int level);
	
	void UpdateEnchantBtn(const UEnchantManager* Enchant);

	void CreateOption(const FString&& infoText, const FString&& formatText, int beforeValue, int afterValue);
	
public:
	void Open();
	
	void SetEnchantEquipTarget(const FName& target);

	void SetEnchantEquipMaterial(const FName& mat);

	UFUNCTION()
	void OnClose();

	UFUNCTION()
	void DoEnchant();
};


