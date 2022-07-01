#include "BI_Spike.h"

#include "MyJrpg/MyLib.h"
#include "NavAreas/NavArea_Null.h"

void UBI_Spike::Init(const TArray<FString>& variable, UInventory* inven)
{
	Super::Init(variable, inven);

	AActor* Owner = Cast<AActor>(GetOuter());

	FTransform Trans;
	USphereComponent* Sphere = Cast<USphereComponent>(Owner->AddComponentByClass(USphereComponent::StaticClass(), false, Trans, false));

	Sphere->SetCollisionProfileName(TEXT("Trigger"));
	Sphere->SetRelativeLocation(FVector(0,0,25));
	Sphere->SetVisibility(false);
	Sphere->SetCanEverAffectNavigation(false);
	Sphere->AreaClass = UNavArea_Null::StaticClass();
	Sphere->SetSphereRadius(300.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &UBI_Spike::OnTriggerStart);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &UBI_Spike::OnTriggerEnd);
}

void UBI_Spike::OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == UMyLib::GetPlayer())
		return;

	Cast<AMonsterPawn>(OtherActor)->TakeDmg(15,nullptr);
}

void UBI_Spike::OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == UMyLib::GetPlayer())
		return;
	Cast<AMonsterPawn>(OtherActor)->TakeDmg(15,nullptr);
}
