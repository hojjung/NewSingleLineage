#include "ItemDDO.h"

#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"

UItemDDO* UItemDDO::GetDDOInst = nullptr;

void UItemDDO::SetDDO(UWidgetBaseElement* ele)
{
	OnDragCancelled.AddDynamic(this, &UItemDDO::OnDragCancel);
	
	DefaultDragVisual = ele->GetImgIcon();

	if(UItemDDO::GetDDOInst)
	{
		FPointerEvent Event;
		UItemDDO::GetDDOInst->DragCancelled(Event);
	}
	
	UItemDDO::GetDDOInst = this;
}


void UItemDDO::OnDragCancel(UDragDropOperation * meSelf)
{
	UItemDDO::GetDDOInst = nullptr;
}

void UItemDDO::OnDrop(UDragDropOperation * meSelf)
{
	UItemDDO::GetDDOInst = nullptr;
}