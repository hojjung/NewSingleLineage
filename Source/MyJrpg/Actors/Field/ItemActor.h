// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Pawns/Components/MinimapIconComp.h"
#include "ItemActor.generated.h"

class UBillboardComponent;
UCLASS()
class MYJRPG_API AItemActor : public AActor, public IFocusable, public INavAgentInterface
{
	GENERATED_BODY()
	
public:	
	AItemActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UMinimapIconComp* m_IconMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBillboardComponent* m_BillboardIcon;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshItem;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* m_Anim;
	FName m_OwnerID;

	FItemSpec m_ItemSpec;

	FNavAgentProperties m_NavProp;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void Init(FName itemID, int countOrLevel);//그럼 어떻게 스택 아이템 처리?

	virtual void OnInteract() override;

	void OnArrived();

	void OnObtain();

	bool HasOwnerTeamID() const;

	virtual FVector GetNavAgentLocation() const override;

	virtual const FNavAgentProperties& GetNavAgentPropertiesRef() const override;

	virtual void GetMoveGoalReachTest(const AActor* MovingActor, const FVector& MoveOffset, FVector& GoalOffset, float& GoalRadius, float& GoalHalfHeight) const override;

	const FItemSpec& GetItemSpec() const;

	virtual FText GetTextInteract() override;

	virtual bool IsInteractable() override;

	virtual UMinimapIconComp* GetIconMeshComp() override;
};

