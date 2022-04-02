#include "Sensor_AlwaysTargetPl.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void USensor_AlwaysTargetPl::UpdateAISensing()
{
	if (m_Owner->GetFocusedTarget())
	{
		return;
	}
	m_Owner->SetFocusedTarget(UMyLib::GetPlayer());
}
