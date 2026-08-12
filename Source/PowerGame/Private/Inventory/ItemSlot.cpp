#include "Inventory/ItemSlot.h"
#include "Inventory/InventoryComponent.h"

#include "UI/Inventory/InventoryPanel.h"
#include "UI/Inventory/InventorySlot.h"

UInventorySlot* FItemSlot::GetSlot(UInventoryPanel* invPanel) const {

    PW_ASSERT(invPanel != nullptr, LogInventory, TEXT("Can't get inventory slot from an invalid inventory panel."));
    PW_ASSERT(invPanel->GetSlot(slotIndex)->GetSlotIndex() == slotIndex, LogInventory, TEXT("Non matching slot indexes."));

    return invPanel->GetSlot(slotIndex);

}