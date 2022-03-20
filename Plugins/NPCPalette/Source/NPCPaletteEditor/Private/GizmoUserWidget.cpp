// My First Hack n Slash


#include "GizmoUserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UGizmoUserWidget::SetImageAndText(UTexture2D* tt2D, FText tt)
{
	m_Image->SetBrushFromTexture(tt2D);

	m_TextName->SetText(tt);
}
