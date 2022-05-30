// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buff_Base.generated.h"

struct FBuffDataRow;
/**
 * 
 */
UCLASS()
class MYJRPG_API UBuff_Base : public UObject
{
	GENERATED_BODY()

protected:
	const FBuffDataRow* m_BuffData;

public:
	virtual void Init(const FBuffDataRow& data);
	
	virtual float GetDuration() const;

	virtual void OnTick(float delta);
	
	virtual void EndBuff();

	FORCEINLINE const FBuffDataRow& GetBuffData() const
	{
		return *m_BuffData;
	}
};
