#include "WidgetCircleIcon.h"

UWidgetCircleIcon::UWidgetCircleIcon(const FObjectInitializer& obj) :Super(obj)
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMat(TEXT("Material'/Game/03_VisualEffect/UIMaterials/M_WidgetCircle.M_WidgetCircle'"));
	m_MatTemplate = FoundMat.Object;
}

void UWidgetCircleIcon::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIcon(m_IconWant);
}

void UWidgetCircleIcon::SetIcon(UTexture2D* t)
{
	if(!m_MatInst)
	{
		m_MatInst=UMaterialInstanceDynamic::Create(m_MatTemplate, this);
	}
	m_MatInst->SetTextureParameterValue(TEXT("Icon"), t);

	m_ImageIcon->SetBrushFromMaterial(m_MatInst);
}
