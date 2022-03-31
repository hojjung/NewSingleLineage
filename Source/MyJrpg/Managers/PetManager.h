#pragma once
#include "CoreMinimal.h"
#include "MyJrpg/Interfaces/PreviewProxy.h"
#include "UObject/NoExportTypes.h"
#include "PetManager.generated.h"

class APreviewActor;
struct FPetRow;
UCLASS()
class MYJRPG_API UPetManager : public UObject, public IPreviewProxy
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnPetChanged);

	FOnPetChanged m_OnPetChanged;
	
protected:
	UPROPERTY()
	APreviewActor* m_PreviewActor;

	TArray<FPetRow*> m_AryPets;

	const FPetRow* m_CrntPet;
	
public:
	void Init();

	const TArray<FPetRow*>& GetPetDatas() const;

	void CreatePreviewActor();

	void EquipPet(const FName& id);

	void EquipPet(const FPetRow& selected);

	void UnEquip();
	
	void ShowPreviewPet(const FPetRow& selected);

	virtual void SetIsTouched(bool b) override;

	virtual void RotatePawn(float delta_x) override;

	void ShowPawn();

	void HidePawn();

	const FPetRow* GetCrntPet() const;
};
