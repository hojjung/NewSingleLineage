// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "NPCPaletteDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class NPCPALETTEEDITOR_API UNPCPaletteDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	UNPCPaletteDataFactory();

	//UNREALED_API static const FName BasicGizmoActor;

	virtual uint32 GetMenuCategories() const override;
	
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	
	virtual bool ShouldShowInNewMenu() const override;
};
