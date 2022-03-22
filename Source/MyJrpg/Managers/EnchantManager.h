#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "UObject/NoExportTypes.h"
#include "EnchantManager.generated.h"

UCLASS()
class MYJRPG_API UEnchantManager : public UObject
{
	GENERATED_BODY()
	
public:
	UEnchantManager();

	DECLARE_MULTICAST_DELEGATE(FOnEnchantChangd);
	
	FOnEnchantChangd m_OnEnchantChanged;

protected:
	FItemSpec* m_CrntTarget;

	FItemSpec* m_CrntMat;
	
public:
	void SetTargetEquip(FItemSpec& target);

	void SetMaterialEquip(FItemSpec& mat);

	FORCEINLINE FItemSpec*  GetCrntTarget() const
	{
		return m_CrntTarget;
	}

	FORCEINLINE FItemSpec* GetCrntMat() const
	{
		return m_CrntMat;
	}

	bool IsAbleTarget(const FItemSpec& target);

	bool IsAbleMaterial(const FItemSpec& material);

	void Clear();
};
