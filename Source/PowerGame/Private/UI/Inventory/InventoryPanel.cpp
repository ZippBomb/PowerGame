#include "UI/Inventory/InventoryPanel.h"
#include "UI/Inventory/InventorySlot.h"

#include "UI/MainHUD.h"

#include "Player/MainPlayerController.h"

#include <Components/WrapBox.h>

inline UInventorySlot* UInventoryPanel::AddSlot(const FItemSlot& data) {

	PW_ASSERT(inventorySlotClass != nullptr, LogUI, TEXT("Inventory slot class must be assigned."));

	UInventorySlot* slot = CreateWidget<UInventorySlot>(GetOwningPlayer(), inventorySlotClass);
	slot->SetData(data);

	slotContainer->AddChild(slot);
	slots.Add(slot);

	return slot;

}
void UInventoryPanel::RemoveSlot(int32 index) {

	PW_ASSERT(index < slots.Num(), LogUI, TEXT("Can't remove an invalid slot index."));

	UInventorySlot* slot = slots[index];

	slot->RemoveFromViewport();
	slotContainer->RemoveChild(slot);

	slots.RemoveAt(index);

}