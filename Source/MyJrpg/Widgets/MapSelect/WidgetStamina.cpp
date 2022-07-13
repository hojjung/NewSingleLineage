#include "WidgetStamina.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStamina::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_StaminaBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_BtnRecharge->OnClicked.AddDynamic(this, &UWidgetStamina::OpenRecharge);

	UMyGameInstance::Get->m_ZoneMove->m_OnStaminaChanged.AddUObject(this, &UWidgetStamina::UpdateStaText);
	
	UpdateStaText();
	
	m_fTimer = 0;
}

void UWidgetStamina::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateRemainTime(InDeltaTime);
}

void UWidgetStamina::UpdateRemainTime(float delta)
{
	float TimeUpdate = 0.f;
	if (UMyGameInstance::Get->m_ZoneMove->GetStamina() == 100)
	{
		m_fTimer = 0.f;
		
		TimeUpdate = 0.f;
	}
	else
	{
		float RemainTime = UMyGameInstance::Get->m_ZoneMove->GetStaminaChargeTime();

		m_fTimer += delta;

		if (m_fTimer > RemainTime)
		{
			UpdateStaText();

			m_fTimer = 0.f;
		}
		TimeUpdate = RemainTime - m_fTimer; 
	}

	FTimespan StaminaTime(0,0,TimeUpdate);
	
	const FString& CultName = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	
	FCulturePtr Culture = FInternationalization::Get().GetCulture(CultName);
	
	m_TextTime->SetText(FText::AsTimespan(StaminaTime,Culture));
}

void UWidgetStamina::OpenRecharge()
{
	PRINTF("Open Cash Shop!");	
}

void UWidgetStamina::UpdateStaText()
{
	int Stamina = UMyGameInstance::Get->m_ZoneMove->GetStamina();
	
	float Per = 0.f;

	if(Stamina > 0)
	{
		Per = (float)Stamina / 100.f;
	}
	
	m_StaminaBar->SetPercent(Per);
	
	FString Str = FString::Printf(TEXT("%d/100"),Stamina);
	
	m_TextAmount->SetText(FText::FromString(Str));
}