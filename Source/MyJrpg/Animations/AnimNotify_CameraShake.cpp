#include "AnimNotify_CameraShake.h"

#include "MyJrpg/MyLib.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!m_ClassCamShake)
	{
		return;
	}

	UWorld* const World = GEngine->GetWorldFromContextObjectChecked(MeshComp);

	if(World->WorldType == EWorldType::EditorPreview)
	{
		return;
	}
     
	// if(World->WorldType != EWorldType::Game)
	// {
	// 	return;
	// }
	
	UMyLib::GetPlayerCon()->ClientStartCameraShake(m_ClassCamShake);	
}
