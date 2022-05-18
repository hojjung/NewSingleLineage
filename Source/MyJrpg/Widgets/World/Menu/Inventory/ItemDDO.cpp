#include "ItemDDO.h"

#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"

UItemDDO* UItemDDO::GetDDOInst = nullptr;

void UItemDDO::SetDDO(UWidgetBaseElement* ele)
{
	OnDragCancelled.AddDynamic(this, &UItemDDO::OnDragCancel);

	OnDrop.AddDynamic(this, &UItemDDO::OnMyDrop);
	
	DefaultDragVisual = ele->GetImgIcon();

	DefaultDragVisual->SetRenderOpacity(0.3f);

	if(UItemDDO::GetDDOInst)
	{
		FPointerEvent Event;
		UItemDDO::GetDDOInst->DragCancelled(Event);
	}
	
	UItemDDO::GetDDOInst = this;
}

const FItemSpec& UItemDDO::GetItem()
{
	if(m_FromInven.Get())
	{
		return m_FromInven->GetItemConstRef(m_nIndex);
	}

	return m_FromEquip->GetEquipItem(m_nIndex);
}

void UItemDDO::BeginDestroy()
{
	Super::BeginDestroy();
	Clear();
}

void UItemDDO::OnDragCancel(UDragDropOperation * meSelf)
{
	Clear();
}

void UItemDDO::OnMyDrop(UDragDropOperation * meSelf)
{
	Clear();
}

void UItemDDO::RemoveItemFromInven()
{
	m_FromInven->NewClearItem(m_nIndex);
}

void UItemDDO::AddItemToSlot(const FItemSpec& item_spec)
{
	m_FromInven->NewAddItem(m_nIndex,item_spec);
}
void UItemDDO::Clear()
{
	if(DefaultDragVisual)
	{
		DefaultDragVisual->SetRenderOpacity(1);
	}
	UItemDDO::GetDDOInst = nullptr;
}