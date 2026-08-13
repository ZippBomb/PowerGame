#include "UI/Inventory/MachineSlot.h"

bool UMachineSlot::CanAcceptDrop(UItemData* droppedItem) const {

	return allowedItems.Contains(droppedItem);

}

bool UMachineSlot::NativeOnDrop(const FGeometry& geometry, const FDragDropEvent& event, UDragDropOperation* operation) {

	uint32 before = quantity;
	if (!Super::NativeOnDrop(geometry, event, operation)) return false;

	onItemAdded.Broadcast(item, quantity - before);

	return true;

}