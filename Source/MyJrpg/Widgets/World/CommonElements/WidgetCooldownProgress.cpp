#include "WidgetCooldownProgress.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

UWidgetCooldownProgress::UWidgetCooldownProgress(const FObjectInitializer& obj):Super(obj)
{
	m_ColorText = FLinearColor::White;
	
	m_fFontSize=22.f;
	//Material'/Game/03_VisualEffect/UIMaterials/M_ProgressRect.M_ProgressRect'
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>FoundMat(TEXT("Material'/Game/03_VisualEffect/UIMaterials/M_ProgressRect.M_ProgressRect'"));
	
	m_MatTemplate = FoundMat.Object;
}

void UWidgetCooldownProgress::SetFontSize(float size)
{
	m_TextCooldown->SetColorAndOpacity(m_ColorText);
	
	FSlateFontInfo Font = m_TextCooldown->Font;
	
	Font.Size=size;
	
	m_TextCooldown->SetFont(Font);
    
	m_fFontSize=size;
}

void UWidgetCooldownProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateProgress();
}

void UWidgetCooldownProgress::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetFontSize(m_fFontSize);

	CreateRenderMat();

	m_ImageCooldown->SetBrushTintColor(m_ColorProgress);
}

void UWidgetCooldownProgress::CreateRenderMat()
{
	if(!m_MatTemplate)
	{
		return;
	}
	
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(m_MatTemplate, this);
	
	m_MatInst = DynMaterial;
    
	if(m_MatInst)
	{
		m_ImageCooldown->SetBrushFromMaterial(m_MatInst);
	}
}

void UWidgetCooldownProgress::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateRenderMat();

	m_TextCooldown->SetText(FText());

	CompleteCooldown();
}

void UWidgetCooldownProgress::StartCooldown(float maxCd)
{
	m_fMaxCD = maxCd;

	m_fCurrentCD=m_fMaxCD;
	
	m_TextCooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_ImageCooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCooldownProgress::CompleteCooldown()
{
	m_TextCooldown->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ImageCooldown->SetVisibility(ESlateVisibility::Collapsed);
	
	m_fCurrentCD=0.f;
}

bool UWidgetCooldownProgress::IsCooldown()
{
	return m_fCurrentCD>0.f; 
}

void UWidgetCooldownProgress::UpdateProgress()
{
	if(m_fCurrentCD<=0.f)
	{
		return;
	}
	
	m_fCurrentCD -= GetWorld()->GetDeltaSeconds();
	
	m_TextCooldown->SetText(FText::FromString(FString::Printf(TEXT("%.1f"),m_fCurrentCD)));
	
	m_MatInst->SetScalarParameterValue(TEXT("Value"),m_fCurrentCD/m_fMaxCD);
	//
	if(m_fCurrentCD<=0.f)
	{
		CompleteCooldown();
	}
}

