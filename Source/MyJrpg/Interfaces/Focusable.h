// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Pawns/Components/MinimapIconComp.h"
#include "UObject/Interface.h"
#include "Focusable.generated.h"


class UInteractBase;

enum class EFocusType
{
	Unit,
	Item,
	Prop
};

UINTERFACE(MinimalAPI)
class UFocusable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYJRPG_API IFocusable
{
	GENERATED_BODY()

public:
	virtual void OnInteract() {};

	virtual bool IsInteractable()
	{
		return true;
	};

	virtual bool IsFocusable()
	{
		return true;
	};

	virtual UMinimapIconComp* GetIconMeshComp() = 0;
};
