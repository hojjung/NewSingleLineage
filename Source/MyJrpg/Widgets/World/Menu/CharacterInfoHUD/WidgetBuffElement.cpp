#include "WidgetBuffElement.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/BuffTable.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "UObject/ConstructorHelpers.h"

void UWidgetBuffElement::SetBuff(const UBuff_Base* buff)
{
	m_BuffDuration->StartCooldown(buff->GetDuration());

	m_ImgIcon->SetBrushFromSoftTexture(buff->GetBuffData().m_Icon);
}