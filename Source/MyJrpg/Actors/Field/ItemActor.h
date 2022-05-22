// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Items/Inventory.h"
#include "ItemActor.generated.h"

class UBillboardComponent;
UCLASS()
class MYJRPG_API AItemActor : public AActor, public IFocusable
{
	GENERATED_BODY()
	
public:	
	AItemActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBillboardComponent* m_BillboardIcon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshItem;
	FName m_OwnerID;

	FItemSpec m_ItemSpec;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void Init(FName itemID, int countOrLevel);//그럼 어떻게 스택 아이템 처리?
	
	void Obtain();

	bool HasOwnerTeamID() const;

	virtual float GetBoundHalfHeight() override;
};

