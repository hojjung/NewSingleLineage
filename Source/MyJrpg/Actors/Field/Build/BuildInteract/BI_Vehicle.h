#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "BI_Vehicle.generated.h"
//설치하고,연료넣어줘야함
UCLASS()
class MYJRPG_API UBI_Vehicle : public UBuildInteractBase
{
	GENERATED_BODY()

public:
	virtual void Init(const TArray<FString>& variable, UInventory* inven) override;

	virtual bool IsEraseable() override;

	virtual void OnInteract() override;

	bool IsVehicleAble();
};
